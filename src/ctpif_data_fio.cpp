#include "ctpif_data_fio.h"


void test_read_thread(void* r) {
  auto saver = (CtpDataReader*) r;
  for(;!saver->writer_is_exited();) {
    saver->got_new_data();

    if (saver->write_buf_was_changed())
      std::cout << "write_buf_was_changed\n";
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
}


void test_write_thread(
        void* w,
        CtpMarketData* data,
        CtpMarketDataInfo* info,
        size_t max_size,
        std::chrono::milliseconds loop_interval) {

  auto writer = (CtpDataWriter*) w;
  std::this_thread::sleep_for(std::chrono::seconds(5));

#ifdef _UNIX
  for (size_t i = 0; i < std::min(max_size, info->di.maxsize); ++i) {
#else
  for (size_t i = 0; i < min(max_size, info->di.maxsize); ++i) {
#endif
    writer->addNewTick(data + i);
#ifdef _DEBUG_
    if (!(i % 1000)) std::cout << i << std::endl;
#endif //_DEBUG_
    std::this_thread::sleep_for(loop_interval);
  }
  printf("closing ... ");
  writer->close_interface();
  printf("done\n");
  std::this_thread::sleep_for(std::chrono::seconds(30));
  printf("exit write thread\n");
}

void test_write_candle_thread(
        void* w,
        CtpMarketData* data,
        CtpMarketDataInfo* info,
        size_t max_size,
        std::chrono::milliseconds loop_interval) {

  auto writer = (CtpCandleWriter*) w;

  char buffer[1024];
  char *line = fgets(buffer, sizeof(buffer), stdin);
  if (*line == 'q') goto exit_writer;

#ifdef _UNIX
  for (size_t i = 0; i < std::min(max_size, info->di.maxsize); ++i) {
#else
  for (size_t i = 0; i < min(max_size, info->di.maxsize); ++i) {
#endif
    printf("TICK: %s %s %s %.02f, %d %d\n",
           data[i].ActionDay, data[i].TradingDay, data[i].UpdateTime, data[i].LastPrice,
           data[i].UpdateMillisec, data[i].Volume);
    writer->addNewTick(data + i);
#ifdef _DEBUG_
    if (!(i % 1000)) std::cout << i << std::endl;
#endif //_DEBUG_
    std::this_thread::sleep_for(loop_interval);
  }
exit_writer:
  printf("closing ... ");
  writer->close_interface();
  printf("done\n");
  std::this_thread::sleep_for(std::chrono::seconds(30));
  printf("exit write thread\n");
}


void test_save_thread(void* r) {
  auto saver = (CtpDataSaver*) r;
  for(;!saver->writer_is_exited();) {
    if (saver->write_buf_was_changed()) {
      std::cout << "write_buf_was_changed\n";
      saver->save();
    }
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
  saver->save(true);
}

void test_read_candle_thread(void* r) {
  auto saver = (CtpCandleReader*) r;
  for(;!saver->writer_is_exited();) {
    saver->got_new_data();

    if (saver->write_buf_was_changed())
      std::cout << "write_buf_was_changed\n";
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
}

void test_save_candle_thread(void* r) {
  auto saver = (CtpCandleSaver*) r;
  for(;!saver->writer_is_exited();) {
    if (saver->write_buf_was_changed()) {
      std::cout << "write_buf_was_changed\n";
      saver->save();
    }
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
  saver->save(true);
}


std::ostream& operator << (std::ostream& out, const NR::CandleBar& bar) {
  out << "OK " << bar.key << ", " << bar.Open << ", " << bar.High << ", "
      << bar.Low << ", " << bar.Close  << ", " << bar.Volume << ", "
      << bar.openInterest << std::endl;
  return out;
}
