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
#include "ctpdataif.h"
#include "fatcatserver.h"

using namespace std;


void read_market_data_thread(void* r) {
  auto reader = (CtpDataInterface*) r;
  for(;!reader->writer_is_exited();) {
    reader->got_new_data();

    if (reader->write_buf_was_changed())
      std::cout << "write_buf_was_changed\n";
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
}


void thread_fatcat_server(UdpSrvReceiver* fatcat) {
  fatcat->start();
}


int main(int argc, char *argv[])
{
  printf("%lu %lu\n", sizeof(Contract), sizeof(InterestChanged));
  return 9;

  if (argc != 2) {
    printf("fct <config filepath>\n");
    return 0;
  }

  GmdParam* param = initConfig(argv[1]);
  if (param == nullptr) {
    cout << "init ctp md if error " << endl;
    return -1;
  }

  auto reader = new CtpDataInterface(param);
  std::thread thread_read(test_read_thread, reader);

  CtpTradeSpi* spi = new CtpTradeSpi(param, param->gtdlog);
  std::thread thread_spi(connect_to_ctp_tdif, spi);
  std::this_thread::sleep_for(std::chrono::seconds(10));

  int ret = spi->login();
  while (!ret && !spi->is_user_logined())
    std::this_thread::sleep_for(std::chrono::seconds(2));

  if (!ret)
      ret = spi->prepare_environment();

  if (ret)
    spi->stop();


  FatcatServer* fatcat = new FatcatServer();
  fatcat->set_ctp_trade_spi(spi);
  std::thread thread_fatcat(thread_fatcat_server, fatcat);

  thread_spi.join();
  thread_read.join();
  thread_fatcat.join();

  SafeDeletePtr(fatcat);
  SafeDeletePtr(reader);
  SafeDeletePtr(spi);

  releaseConfig(param);

  return (0);
}

