#include "ctpif_global.h"
#include "ctpif_data.h"
#include "ctpif.h"
#include "ctpmdspi.h"
#include "ctptdspi.h"
#include "rapidjson/document.h"
#include "str.h"
#include "utility.h"

#include <thread>
#include <iostream>
#include <signal.h>
#include <stdio.h>

// check memory leak
#ifdef _WINDOWS
#ifdef _DEBUG_
#ifndef DBG_NEW
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif // _DEBUG_
#endif //_WINDOWS

using namespace std;

CtpSpeedTest* param = nullptr;
CtpTestTradeSpi* spi = nullptr;

static bool run = false;

static int connect_to_test_tdif(CtpTestTradeSpi * trade_spi) {
    if (!trade_spi)
      return -100;

    return trade_spi->run();
  }


LARGE_INTEGER frequency;        // ticks per second
LARGE_INTEGER t1, t2;           // ticks
double elapsedTime;


CTPORDER od;
int order_ref = 0;
bool order_returned = false;
bool is_insert = false;

static int get_order(CTPORDER* order) {
    if (order
            && atoi(order->OrderRef) == order_ref
            && order->OrderSubmitStatus == '3'
            && (order->OrderStatus == '3'
                ||
                order->OrderStatus == '5')) {
        memcpy(&od, order, sizeof(CTPORDER));
        // stop timer
        QueryPerformanceCounter(&t2);

        // compute and print the elapsed time in millisec
        elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
        if (is_insert && order->OrderStatus == '3')
            printf("\tinserted spent: %0.2lf ms\n", elapsedTime);
        if (!is_insert && order->OrderStatus == '5')
            printf("\tcanceled spent: %0.2lf ms\n", elapsedTime);
        order_returned = true;
    }
    return 0;
}

static void thread_testing(CtpSpeedParam* ctp_param) {
  // connect to trade insterface and get something then break
  spi = new CtpTestTradeSpi(ctp_param, param->logpath.c_str());
  spi->set_order_return(get_order);

  std::thread thCtptd = std::thread{connect_to_test_tdif, spi};
  std::this_thread::sleep_for(std::chrono::seconds(10));
  spi->login();
  // std::this_thread::sleep_for(std::chrono::seconds(10));
  // gtdspi->prepare_environment();

  for (;run;)
    std::this_thread::sleep_for(std::chrono::seconds(10));

  spi->logout();
  std::this_thread::sleep_for(std::chrono::seconds(10));

  thCtptd.detach();
  SafeDeletePtr(spi);
}

void got_signal(int s) {
  if (s == SIGINT) {
    printf("got signal %d\n", s);
    run = false;
  }
}


int main(int argc, char *argv[])
{
#ifdef _WINDOWS
#ifdef _DEBUG_
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  // _CrtDumpMemoryLeaks();
#endif //_DEBUG_
#endif //_WINDOWS

  if (argc != 2) {
      cout << "Usage speed <config filepath>\n";
      exit(0);
  }

  // get ticks per second
  QueryPerformanceFrequency(&frequency);

  int ret;
  param = initSpeedParameter(argv[1]);
  ret = (param == nullptr);
  if (ret) {
      cout << "config file error\n";
      exit(-1);
  }

  std::chrono::milliseconds interval{param->interval};

  double* report = new double[param->params.size() * param->cases.size()];

  for (size_t i = 0; (!ret) && i < param->params.size(); ++i) {
      order_ref = -1;
      printf("Please wait to connecting to %s ...... \n", param->params[i]->name.c_str());
      run = true;
      std::thread thread_tdif(thread_testing, param->params[i]);

      std::this_thread::sleep_for(std::chrono::seconds(10));

      for (size_t j = 0; j < param->cases.size(); ++j) {
          double average = 0;
          for (int k = 0; k < param->loop_times; k++) {
              printf("insert an order to %s, %s, %.02lf, %d, %d ...\n",
                     param->cases[j]->name.c_str(),
                     param->cases[j]->instrument.c_str(),
                     param->cases[j]->price,
                     param->cases[j]->dir,
                     param->cases[j]->volume);
              order_returned = false;
              order_ref = spi->get_order_ref();
              is_insert = true;
              // start timer
              QueryPerformanceCounter(&t1);
              spi->order_insert_limitd(param->cases[j]->instrument.c_str(),
                                       '0' + param->cases[j]->dir,
                                       param->cases[j]->price,
                                       param->cases[j]->volume);
              while(!order_returned)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

              average += elapsedTime;

              std::this_thread::sleep_for(interval);
              printf("recall the order ...\n");
              order_returned = false;
              is_insert = false;
              QueryPerformanceCounter(&t1);
              spi->order_recall(&od);
              while(!order_returned)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

              average += elapsedTime;
              std::this_thread::sleep_for(interval);
          }

          average = average / (double)(param->loop_times) * 0.5;
          printf("Average time: %.02lf ms\n", average);
          report[i * param->cases.size() + j] = average;
          std::this_thread::sleep_for(interval);
      }
      run = false;
      printf("Please wait for disconnecting from %s\n", param->params[i]->name.c_str());
      thread_tdif.join();
  }

  //print report
  printf ("\nprint report:\n");

  char timenow[MAX_PATH];
  time_t t = time(0);
  struct tm tt;
  localtime_s(&tt, &t);
  snprintf(timenow, MAX_PATH, "%04d/%02d/%02d %02d:%02d:%02d",
           tt.tm_year + 1900,
           tt.tm_mon + 1,
           tt.tm_mday,
           tt.tm_hour,
           tt.tm_min,
           tt.tm_sec);

  FILE* fp = nullptr;
  if (fopen_s(&fp, "result.csv", "a"))
      goto error_exit;

  fprintf(stdout, "CTP,datetime");
  fprintf(fp, "CTP,datetime");

  for (size_t j = 0; j < param->cases.size(); ++j) {
    fprintf(stdout, ",%s", param->cases[j]->name.c_str());
    fprintf(fp, ",%s", param->cases[j]->name.c_str());
  }
  fprintf(stdout, "\n");
  fprintf(fp, "\n");

  for (size_t i = 0; (!ret) && i < param->params.size(); ++i) {
      fprintf(stdout, "%s,%s", param->params[i]->name.c_str(), timenow);
      fprintf(fp, "%s,%s", param->params[i]->name.c_str(), timenow);
      for (size_t j = 0; j < param->cases.size(); ++j) {
        fprintf(stdout, ",%.02lf", report[i * param->cases.size() + j]);
        fprintf(fp, ",%.02lf", report[i * param->cases.size() + j]);
      }
      fprintf(stdout, "\n");
      fprintf(fp, "\n");
  }
  fclose(fp);

error_exit:
  delete [] report;
  releaseSpeedParameter(param);

  return ret;
}
