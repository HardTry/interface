#include "ctpif.h"
#include "ctpmdspi.h"
#include "ctptdspi.h"
#include "mem.h"
#include "rapidjson/document.h"
#include "str.h"
#include "utility.h"
#include <chrono>
#include <stdio.h>
#include <thread>
#include "ctpif_data_fio.h"



using namespace std;




int main(int argc, char *argv[])
{
  // 408
  // printf("%lu\n", sizeof(CtpMarketData));

  if (argc != 2) {
    printf("datasimu <config filepath>\n");
    return 0;
  }

  GmdParam* param = initConfig(argv[1]);
  if (param == nullptr) {
    cout << "init ctp md if error " << endl;
  }

  //1. auto saver = new CtpDataSaver(param);
  //std::thread thread_read(test_read_thread, saver);
  //std::thread thread_read(test_save_thread, saver);

  //3. auto saver = new CtpCandleReader(param);
  //4. std::thread thread_read(test_read_candle_thread, saver);

  auto saver = new CtpCandleSaver(param);
  std::thread thread_read(test_save_candle_thread, saver);
  thread_read.join();

  SafeDeletePtr(saver);
  releaseConfig(param);

  //cmp /app/sean/data/tick/ru1701-20161118.tick  /data/sean/tick/ru1701-20161118.tick
  return (0);
}

