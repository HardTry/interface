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

