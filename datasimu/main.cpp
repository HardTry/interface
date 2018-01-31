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

#define EXPECT_EQ(x, y) if ((x) != (y)) {\
  cout << "error not eq, " << (x) << " .vs. " << (y) << ", in " << __FILE__ << " at line " << __LINE__ << endl;\
  return;\
}


using namespace std;

// static CtpMarketData *test_data = 0;
// static char day[] = "20160905";
// static char tt[] = "11:23:36";
// static size_t all_data_size = 0;
// static bool bSaveCtpmmd = true;




int main(int argc, char *argv[])
{
  if (argc != 5) {
    printf("datasimu <config filepath> <loop interval> <data filepath> <data number>\n");
    printf("config filepath: the path of config file\n"
           "loop interval: the time interval of wrting-loop\n"
           "data filepath: the path of tickdata file\n\n");
    return 0;
  }

  int ret = 0;

  GmdParam* param = initConfig(argv[1]);
  if (param == nullptr) {
    cout << "init ctp md if error " << endl;
  }

  std::chrono::milliseconds loop_interval = std::chrono::milliseconds(atoi(argv[2]));

  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // ! remove all history data
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  char command[MAX_PATH];
  snprintf(command, MAX_PATH, "rm -f %s/*", param->data_store_path);
  // cout << command << endl;
  system(command);
  snprintf(command, MAX_PATH, "rm -f %s/*", param->data_rtpath);
  // cout << command << endl;
  system(command);
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // ! remove all history data
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  CtpMarketData* data = 0;
  CtpMarketDataInfo* info =0;
  ret = load_data_from_file(argv[3], &data, &info);
  if (ret) return ret;


  auto writer = new CtpDataWriter(param);
  // auto saver = new CtpDataSaver(param);

  std::thread thread_write(test_write_thread, writer, data, info, atoi(argv[4]), loop_interval);
  // std::thread thread_read(test_read_thread, saver);

  // thread_read.join();
  thread_write.join();

  // SafeDeletePtr(saver);
  SafeDeletePtr(writer);

  SafeDeletePtr(info);
  SafeDeleteAry(data);

  releaseConfig(param);

  return (0);
}
