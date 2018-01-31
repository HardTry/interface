#include "ctpif_global.h"
#include "utility_global.h"
#include "mderr.h"
#include "str.h"
#include "utility.h"

#include "ctpmdspi.h"
#include "ctptdspi.h"

#include "mem.h"
#include "ctpif_data.h"

#include "rapidjson/document.h"
#include <chrono>
#include <gtest/gtest.h>
#include <stdio.h>
#include <thread>
#include "ctpif_data_fio.h"

// static char confpath[MAX_PATH];

#ifdef _WINDOWS
// check memory leak

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




//
// TEST(ctp_market_data, connect) {
// #ifdef _WINDOWS
//   int result = initCtpMdIf(confpath);
// #else
//   int result = initCtpMdIf(confpath);
// #endif
//
//   EXPECT_EQ(result, 0);
//   gmdmem = new CtpMdMemory;
//   result = gmdmem->initMemory();
//   EXPECT_EQ(result, 0);
//
//   gmdspi = new CtpMdSpi;
//   std::thread t(thread_connect_to_ctpmd);
//
//   std::this_thread::sleep_for(std::chrono::seconds(60));
//
//   gspi->Release();
//   std::this_thinstgeterread::sleep_for(std::chrono::seconds(6));
//   SafeDeletePtr(gspi);
//
//   result = gmdmem->destroyMemory();
//   EXPECT_EQ(result, 0);
//   SafeDeletePtr(gmdmem);
//   releaseCtpMdIf();
// }
//

// TEST(ctp_trade_api, the_trade_api_class) {
//   CtpTradeSpi *api = new CtpTradeSpi(gmd_param->gtdlog);
//   SafeDeletePtr(api);
// }

//
// TEST(ctp_trade_api, connect) {
// #ifdef _WINDOWS
//   int result = initCtpMdIf(confpath);
// #else
//   int result = initCtpMdIf(confpath);
// #endif
//   EXPECT_EQ(result, 0);
//
//   gtrade = new CtpTradeSpi(gmd_param->gtdlog);
//   std::thread t(thread_connect_to_ctptrade);
//
//   std::this_thread::sleep_for(std::chrono::seconds(600));
//
//   gtrade->stop();
//   //t.join();
//   std::this_thread::sleep_for(std::chrono::seconds(60));
//
//   SafeDeletePtr(gtrade);
//
//   releaseCtpMdIf();
// }
//


// #include "ctpdi.h"
//
// void print (const char* inst, const size_t len) {
//     cout << inst << len << endl;
// }
//
//
// void listen_kindle(const char* instrument, KINDLE* kindle, int period) {
//   cout << instrument << kperiod_postfix[period + 4] << ", " << kindle->UpdateTime << ", " << kindle->open << ", " << kindle->volume << endl;
// }
//
//
// void  thread_listen_kindle() {
//   start_listen(listen_kindle);
// }
//
// TEST(ctpdi, test) {
//   cout << "please run datasimu before start this test\n"
//           "./datasimu <json config file> 10 3\n";
//
// #ifdef _WINDOWS
//   int result = initCtpMdIf(confpath);
// #else
//   int result = initCtpMdIf(confpath);
// #endif
//   EXPECT_EQ(result, 0);
//
//   // Ctpdi* ctpdi = new Ctpdi();
//   // EXPECT_NE(ctpdi, nullptr);
//   //
//   // EXPECT_EQ(0, ctpdi->open_data_interface());
//   // EXPECT_EQ(0, ctpdi->close_data_interface());
//   //
//   // SafeDeletePtr(ctpdi);
//
//
//   EXPECT_EQ(0, open_data_interface());
//
//   bool run = true;
//   for (;run;) {
//      KINDLE* kindle = 0;
//      size_t len = 0, oldlen = 0, newlen = 0;
//
//      EXPECT_NE((void*)NULL, kindle = get_kindle_data( "CF701", "m1", &len, &oldlen, &newlen));  print("CF701 , m1: ", len);
//
//      if (len > 180) run = false;
//      //for (size_t i = 0; i < len; ++i)
//      //    cout << i << ", " << kindle[i].UpdateTime << ", " << kindle[i].open << ", " << kindle[i].vol << endl;
//
//      // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "IF1611","m1", &len, &oldlen, &newlen));  print("IF1611, m1: ", len);
//      // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "SR701", "m1", &len, &oldlen, &newlen));  print("SR701 , m1: ", len);
//      // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ag1701","m1", &len, &oldlen, &newlen));  print("ag1701, m1: ", len);
//      // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "au1701","m1", &len, &oldlen, &newlen));  print("au1701, m1: ", len);
//      // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "i1710", "m1", &len, &oldlen, &newlen));  print("i1710 , m1: ", len);
//      // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "j1701", "m1", &len, &oldlen, &newlen));  print("j1701 , m1: ", len);
//      // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "m1701", "m1", &len, &oldlen, &newlen));  print("m1701 , m1: ", len);
//      // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ni1701","m1", &len, &oldlen, &newlen));  print("ni1701, m1: ", len);
//      // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "p1701", "m1", &len, &oldlen, &newlen));  print("p1701 , m1: ", len);
//      // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "rb1701","m1", &len, &oldlen, &newlen));  print("rb1701, m1: ", len);
//      // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ru1701","m1", &len, &oldlen, &newlen));  print("ru1701, m1: ", len);
//      // cout << endl;
//
//      // if (!(i % 5)) {
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "CF701", "m5", &len, &oldlen, &newlen));  print("CF701 , m5: ", len);
//
//      EXPECT_NE((void*)NULL, kindle = get_kindle_data( "IF1611","m5", &len, &oldlen, &newlen));  print("IF1611, m5: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "SR701", "m5", &len, &oldlen, &newlen));  print("SR701 , m5: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ag1701","m5", &len, &oldlen, &newlen));  print("ag1701, m5: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "au1701","m5", &len, &oldlen, &newlen));  print("au1701, m5: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "i1710", "m5", &len, &oldlen, &newlen));  print("i1710 , m5: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "j1701", "m5", &len, &oldlen, &newlen));  print("j1701 , m5: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "m1701", "m5", &len, &oldlen, &newlen));  print("m1701 , m5: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ni1701","m5", &len, &oldlen, &newlen));  print("ni1701, m5: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "p1701", "m5", &len, &oldlen, &newlen));  print("p1701 , m5: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "rb1701","m5", &len, &oldlen, &newlen));  print("rb1701, m5: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ru1701","m5", &len, &oldlen, &newlen));  print("ru1701, m5: ", len);
//        // cout << endl;
//      // }
//
//      // if (!(i % 11)) {
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "CF701", "m10", &len, &oldlen, &newlen));  print("CF701 , m10: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "IF1611","m10", &len, &oldlen, &newlen));  print("IF1611, m10: ", len);
//        EXPECT_NE((void*)NULL, kindle = get_kindle_data( "SR701", "m10", &len, &oldlen, &newlen));  print("SR701 , m10: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ag1701","m10", &len, &oldlen, &newlen));  print("ag1701, m10: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "au1701","m10", &len, &oldlen, &newlen));  print("au1701, m10: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "i1710", "m10", &len, &oldlen, &newlen));  print("i1710 , m10: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "j1701", "m10", &len, &oldlen, &newlen));  print("j1701 , m10: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "m1701", "m10", &len, &oldlen, &newlen));  print("m1701 , m10: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ni1701","m10", &len, &oldlen, &newlen));  print("ni1701, m10: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "p1701", "m10", &len, &oldlen, &newlen));  print("p1701 , m10: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "rb1701","m10", &len, &oldlen, &newlen));  print("rb1701, m10: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ru1701","m10", &len, &oldlen, &newlen));  print("ru1701, m10: ", len);
//        // cout << endl;
//      // }
//
//      // if (!(i % 16)) {
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "CF701", "m15", &len, &oldlen, &newlen));  print("CF701 , m15: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "IF1611","m15", &len, &oldlen, &newlen));  print("IF1611, m15: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "SR701", "m15", &len, &oldlen, &newlen));  print("SR701 , m15: ", len);
//        EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ag1701","m15", &len, &oldlen, &newlen));  print("ag1701, m15: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "au1701","m15", &len, &oldlen, &newlen));  print("au1701, m15: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "i1710", "m15", &len, &oldlen, &newlen));  print("i1710 , m15: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "j1701", "m15", &len, &oldlen, &newlen));  print("j1701 , m15: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "m1701", "m15", &len, &oldlen, &newlen));  print("m1701 , m15: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ni1701","m15", &len, &oldlen, &newlen));  print("ni1701, m15: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "p1701", "m15", &len, &oldlen, &newlen));  print("p1701 , m15: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "rb1701","m15", &len, &oldlen, &newlen));  print("rb1701, m15: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ru1701","m15", &len, &oldlen, &newlen));  print("ru1701, m15: ", len);
//        // cout << endl;
//      // }
//
//      // if (!(i % 31)) {
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "CF701", "m30", &len, &oldlen, &newlen));  print("CF701 , m30: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "IF1611","m30", &len, &oldlen, &newlen));  print("IF1611, m30: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "SR701", "m30", &len, &oldlen, &newlen));  print("SR701 , m30: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ag1701","m30", &len, &oldlen, &newlen));  print("ag1701, m30: ", len);
//        EXPECT_NE((void*)NULL, kindle = get_kindle_data( "au1701","m30", &len, &oldlen, &newlen));  print("au1701, m30: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "i1710", "m30", &len, &oldlen, &newlen));  print("i1710 , m30: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "j1701", "m30", &len, &oldlen, &newlen));  print("j1701 , m30: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "m1701", "m30", &len, &oldlen, &newlen));  print("m1701 , m30: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ni1701","m30", &len, &oldlen, &newlen));  print("ni1701, m30: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "p1701", "m30", &len, &oldlen, &newlen));  print("p1701 , m30: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "rb1701","m30", &len, &oldlen, &newlen));  print("rb1701, m30: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ru1701","m30", &len, &oldlen, &newlen));  print("ru1701, m30: ", len);
//        // cout << endl;
//      // }
//
//      // if (!(i % 61)) {
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "CF701", "h1", &len, &oldlen, &newlen));  print("CF701 , h1: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "IF1611","h1", &len, &oldlen, &newlen));  print("IF1611, h1: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "SR701", "h1", &len, &oldlen, &newlen));  print("SR701 , h1: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ag1701","h1", &len, &oldlen, &newlen));  print("ag1701, h1: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "au1701","h1", &len, &oldlen, &newlen));  print("au1701, h1: ", len);
//        EXPECT_NE((void*)NULL, kindle = get_kindle_data( "i1710", "h1", &len, &oldlen, &newlen));  print("i1710 , h1: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "j1701", "h1", &len, &oldlen, &newlen));  print("j1701 , h1: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "m1701", "h1", &len, &oldlen, &newlen));  print("m1701 , h1: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ni1701","h1", &len, &oldlen, &newlen));  print("ni1701, h1: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "p1701", "h1", &len, &oldlen, &newlen));  print("p1701 , h1: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "rb1701","h1", &len, &oldlen, &newlen));  print("rb1701, h1: ", len);
//        // EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ru1701","h1", &len, &oldlen, &newlen));  print("ru1701, h1: ", len);
//
//        EXPECT_NE((void*)NULL, kindle = get_kindle_data( "m1701", "h4", &len, &oldlen, &newlen));  print("m1701 , h4: ", len);
//        EXPECT_NE((void*)NULL, kindle = get_kindle_data( "ni1701","D1", &len, &oldlen, &newlen));  print("ni1701, D1: ", len);
//        EXPECT_NE((void*)NULL, kindle = get_kindle_data( "p1701", "W1", &len, &oldlen, &newlen));  print("p1701 , W1: ", len);
//        EXPECT_NE((void*)NULL, kindle = get_kindle_data( "rb1701","M1", &len, &oldlen, &newlen));  print("rb1701, M1: ", len);
//        cout << endl;
//      // }
//
//     std::this_thread::sleep_for(std::chrono::seconds(10));
//   }
//
//   // std::thread tStartListen(thread_listen_kindle);
//   // std::this_thread::sleep_for(std::chrono::seconds
//   //                             (30));
//   // stop_listen();
//   // tStartListen.join();
//
//   EXPECT_EQ(0, close_data_interface());
//
//   releaseCtpMdIf();
// }
//


int main(int argc, char **argv) {

#ifdef _WINDOWS
#ifdef _DEBUG_
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  _CrtDumpMemoryLeaks();
#endif //_DEBUG_
#endif //_WINDOWS

  testing::InitGoogleTest(&argc, argv);

  // bool check_for_leaks = false;
  // if (argc > 1 && strcmp(argv[1], "--check_for_leaks") == 0 )
  //   check_for_leaks = true;
  // else
  //   printf("%s\n", "Run this program with --check_for_leaks to enable "
  //          "custom leak checking in the te    cout << "generator " << i <<
  //          endl;sts.");
  //
  // // If we are given the --check_for_leaks command line flag, installs the
  // // leak checker.    cout << "generator " << i << endl;
  // if (check_for_leaks) {
  //   TestEventListeners& listeners = UnitTest::GetInstance()->listeners();
  //    cout << "generator " << i << endl;
  //   // Adds the leak checker to the end of the test event listener list,
  //   // after the default text output printer and the default XML report
  //   // generator.    cout << "generator " << i << endl;
  //   //
  //   // The order is important - it ensures that failures generated in the
  //   // leak checker's OnTestEnd() method a    cout << "generator " << i <<
  //   endl;re processed by the text and XML
  //   // printers *before* their OnTestEnd() methods are called, such that
  //   // they are attributed to 2the right test. Remember that a listener
  //   // receives an OnXyzStart event *after    cout << "generator " << i <<
  //   endl;* listeners preceding it in the
  //   // list received that even2t, and receives an OnXyzEnd event *before*
  //   // listeners preceding it.
  //   //    cout << "generator " << i << endl;
  //   // We don't need to worry about deleting the new listener later, as
  //   // Google Test will do it.
  //   listeners.Append(new LeakChecker);
  // }
  return RUN_ALL_TESTS();
}

static const char* config = {"/app/sean/bin/gom/conf/gmd-simnow.json"};
static const char* raw_data_file = {"/data/sean/tick/ru1701-20161118.tick"};
static const auto loop_interval = std::chrono::milliseconds(10);
static const size_t max_data_amount = 30000000;
static const char* tick_data_file = {"/app/sean/data/tick/ru1701-20161118.tick"};
static const char* ctpmd_data_file = {"/app/sean/data/rt/ru1701-.ctpmd"};

TEST(dataset, ctp_interface) {
    int ret = 0;

    GmdParam* param = initConfig(config);
    EXPECT_NE(param, nullptr);

    CtpMarketData* data = 0;
    CtpMarketDataInfo* info =0;
    ret = load_data_from_file(raw_data_file, &data, &info);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(data, nullptr);
    EXPECT_EQ(info->di.maxsize, 39196);

    /*auto writer = new CtpDataWriter(param);
    EXPECT_NE(writer, nullptr);

    auto saver = new CtpDataReader(param);
    EXPECT_NE(saver, nullptr);


    std::thread thread_write(test_write_thread, writer, data, info, max_data_amount, loop_interval);
    std::thread thread_read(test_save_thread, saver);

    thread_read.join();
    thread_write.join();

    SafeDeletePtr(saver);
    SafeDeletePtr(writer);*/

    SafeDeletePtr(info);
    SafeDeleteAry(data);

    releaseConfig(param);

    ret = compare_files(raw_data_file, tick_data_file);
    EXPECT_EQ(ret, 0);

    FILE* fp1 = fopen(raw_data_file, "rb");
    EXPECT_NE(fp1, nullptr);
    FILE* fp2 = fopen(ctpmd_data_file, "rb");
    EXPECT_NE(fp2, nullptr);

    fseek(fp1, 0, SEEK_END);
    long s1 = ftell(fp1) / sizeof(CtpMarketData);
    fseek(fp1, 0, SEEK_SET);

    fseek(fp2, 0, SEEK_END);
    long s2 = ftell(fp2) / sizeof(CTPMD);
    fseek(fp2, 0, SEEK_SET);

    EXPECT_EQ(s1, s2);

    for (long i = 0; i < s1; ++i) {
      CtpMarketData cmd;
      CTPMD ctpmd;
      EXPECT_EQ(fread(&cmd, sizeof(CtpMarketData), 1, fp1), 1);
      EXPECT_EQ(fread(&ctpmd, sizeof(CTPMD), 1, fp2), 1);
      EXPECT_EQ(cmd.AskPrice1, ctpmd.AskPrice1);
      EXPECT_EQ(cmd.BidPrice1, ctpmd.BidPrice1);
      EXPECT_EQ(cmd.LastPrice, ctpmd.LastPrice);
      EXPECT_EQ(cmd.AskVolume1, ctpmd.AskVolume1);
      EXPECT_EQ(cmd.BidVolume1, ctpmd.BidVolume1);
      EXPECT_EQ(cmd.Volume, ctpmd.Volume);
      EXPECT_EQ(cmd.OpenInterest, ctpmd.OpenInterest);
      EXPECT_EQ(cmd.Turnover, ctpmd.Turnover);
    }


    fclose(fp2);
    fclose(fp1);
}
