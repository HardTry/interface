#include <iostream>
#include <thread>
#include "ctpif.h"

using namespace std;


int main(int argc, char *argv[])
{
  if (argc != 2) {
    cout << "Usage kindle(d) <json config file>\n";
    return (0);
  }

  GmdParam* param = initConfig(argv[1]);
  if (param == nullptr) {
    cout << "init ctp md if error " << endl;
    return -1;
  }

  uint8_t j = 0;
  time_t today_exit = 0;

  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (!(j % 240))
        today_exit = get_exit_time(param->exit_time.c_str());

    if (time(0) >= today_exit && time(0) <= today_exit + 3600 * 2)
      break;

    ++j;
  }

  releaseConfig(param);

  return 0;
}
