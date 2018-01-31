#include "ctpmdspi.h"
#include "dataset.h"
#include "ctpif_global.h"
#include "ctpif_data.h"
#include "ctpif.h"
#include "mem.h"
#include "str.h"
#include "utility.h"
#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <regex>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <time.h>
#include <vector>

using namespace std;
// static char confpath[MAX_PATH];

int main(int argc, char **argv) {

#ifdef _WINDOWS
#ifdef _DEBUG_
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  _CrtDumpMemoryLeaks();
#endif //_DEBUG_
#endif //_WINDOWS

  // if (argc != 2) {
  //   printf("Usage testing <json config file>\n");
  //   return (0);
  // }
  //
  // strcpy_s(confpath, MAX_PATH, argv[1]);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// TEST(ctp_json_config, matlab) {
//   GmdParam* param = initConfig(nullptr);
//   EXPECT_EQ(param, nullptr);
//   // printf("%s\n", getErrorString(result));
// }

//
// TEST(DataInterfaceMemory, init_writer_memory) {
//     EXPECT_EQ(0, initCtpMdIf(confpath));
//     auto mdWriter = new CtpWriter(ctp_param->instrument);
//
//     EXPECT_EQ(0, mdWriter->initMemory(
//                   gmd_param->max_tick_per_sec * gmd_param->save_tickbuf_time * 60,
//                   gmd_param->max_time_long * gmd_param->max_tick_per_sec,
//                   gmd_param->tick_buffer_num,
//                   gmd_param->data_rtpath,
//                   ctp_param->shred_mem_name.c_str()));
//     EXPECT_EQ(0, mdWriter->destroyMemory());
//     SafeDeletePtr(mdWriter);
//     releaseCtpMdIf();
// }
//
// TEST(CtpMdData, read_ctp_tick) {
//   EXPECT_EQ(0, initCtpMdIf(confpath));
//   CtpMarketData* data = 0;
//   CtpMarketDataInfo* info = 0;
//   EXPECT_EQ(0, load_ctp_tick_from_file("/work/sean/projects/build/data/ru1701-20161118.tick", &data, &info));
//
//   EXPECT_NE(data, nullptr);
//   EXPECT_NE(info, nullptr);
//   EXPECT_GT(info->di.maxsize, 0);
//
//   bool b = invalid_data(data[0]);
//
//   EXPECT_EQ(true, b);
//
//   SafeDeleteAry(data);
//   SafeDeletePtr(info);
//
//   releaseCtpMdIf();
// }
//
//
//
// TEST(DataInterfaceMemory, init_readble_memory) {
//     EXPECT_EQ(0, initCtpMdIf(confpath));
//     auto mdWriter = new CtpWriter(ctp_param->instrument, true, true);
//     EXPECT_EQ(0, mdWriter->initMemory(
//                   gmd_param->max_tick_per_sec * gmd_param->save_tickbuf_time * 60,
//                   gmd_param->max_time_long * gmd_param->max_tick_per_sec,
//                   gmd_param->tick_buffer_num,
//                   gmd_param->data_rtpath,
//                   ctp_param->shred_mem_name.c_str()));
//
//
//     auto mdReader = new CtpReader(ctp_param->instrument, false, true);
//     EXPECT_EQ(0, mdReader->initMemory(
//                   gmd_param->max_tick_per_sec * gmd_param->save_tickbuf_time * 60,
//                   gmd_param->max_time_long * gmd_param->max_tick_per_sec,
//                   gmd_param->tick_buffer_num,
//                   gmd_param->data_rtpath,
//                   ctp_param->shred_mem_name.c_str()));
//
//
//     EXPECT_EQ(0, mdReader->destroyMemory());
//     SafeDeletePtr(mdReader);
//
//     EXPECT_EQ(0, mdWriter->destroyMemory());
//     SafeDeletePtr(mdWriter);
//     releaseCtpMdIf();
// }
//

//
// TEST(DataSetMemory, read_write) {
//   CtpMarketData* data = 0;
//   CtpMarketDataInfo* info = 0;
//
//   // EXPECT_EQ(0, load_data_from_file("/work/sean/projects/build/data/ru1701-20161118.tick", &data, &info));
//   EXPECT_EQ(0, load_data_from_file("/data/znl/gom/data/tick/ru1701-20161118.tick", &data, &info));
//
//   SharedMemory* memWriter = SharedMemory::getMemMapping();
//   EXPECT_EQ(0, memWriter->OpenWrite("/tmp", "test dataset memory", DATASET(CtpMarketData)::get_memory_size(info->di.maxsize)));
//   EXPECT_NE(nullptr, memWriter->GetDataPtr());
//
//   SharedMemory* memReader = SharedMemory::getMemMapping();
//   EXPECT_EQ(0, memReader->OpenRead("test dataset memory", DATASET(CtpMarketData)::get_memory_size(info->di.maxsize)));
//   EXPECT_NE(nullptr, memReader->GetDataPtr());
//
//   auto writer = new DATASET(CtpMarketData)("CtpMarketData", 'w');
//   auto reader = new DATASET(CtpMarketData)("CtpMarketData", 'r');
//   EXPECT_NE(nullptr, writer);
//   EXPECT_NE(nullptr, reader);
//
//   writer->set_memory(memWriter->GetDataPtr(), nullptr, info->di.maxsize, 'w');
//   for (size_t i = 0; i < info->di.maxsize; ++i) {
//     writer->append(data + i);
//   }
//
//   for (size_t i = 0; i < info->di.maxsize; ++i) {
//     EXPECT_EQ(0, memcmp(data + i, writer->at(i), sizeof(CtpMarketData)));
//   }
//
//   reader->set_memory(memReader->GetDataPtr(), nullptr, info->di.maxsize, 'r');
//   for (size_t i = 0; i < info->di.maxsize; ++i) {
//     EXPECT_EQ(0, memcmp(data + i, reader->at(i), sizeof(CtpMarketData)));
//   }
//
//   SafeDeletePtr(reader);
//   SafeDeletePtr(writer);
//
//   memReader->CloseFile();
//   SafeDeletePtr(memReader);
//
//   memWriter->CloseFile();
//   SafeDeletePtr(memWriter);
//
//   SafeDeleteAry(data);
//   SafeDeletePtr(info);
// }
//
// static void thread_write_tick(CtpMarketData* data, CtpMarketDataInfo* info) {
//     SharedMemory* memWriter = SharedMemory::getMemMapping();
//     EXPECT_EQ(0, memWriter->OpenWrite("/tmp", "test dataset memory", DATASET(CtpMarketData)::get_memory_size(info->di.maxsize)));
//     EXPECT_NE(nullptr, memWriter->GetDataPtr());
//
//     auto writer = new DATASET(CtpMarketData)("CtpMarketData", 'w');
//     EXPECT_NE(nullptr, writer);
//
//     std::this_thread::sleep_for(std::chrono::seconds(10));
//
//     writer->set_memory(memWriter->GetDataPtr(), nullptr, info->di.maxsize, 'w');
//     for (size_t i = 0; i < info->di.maxsize; ++i) {
//       writer->append(data + i);
//     }
//
//     SafeDeletePtr(writer);
//     memWriter->CloseFile();
//     SafeDeletePtr(memWriter);
//
// }
//
// static int new_tick(void* dataset, void* data, int64_t pos) {
//   // auto obj = (DATASET(CtpMarketData)*) dataset;
//   // cout << pos << " , " << (CtpMarketData*)data << ", " << obj->info->di.pos << " vs " << obj->nrow << endl;
//   return 0;
// }
//
// TEST(DataSetMemory, read_write_thread) {
//     CtpMarketData* data = 0;
//     CtpMarketDataInfo* info = 0;
//     // EXPECT_EQ(0, load_data_from_file("/work/sean/projects/build/data/ru1701-20161118.tick", &data, &info));
//     EXPECT_EQ(0, load_data_from_file("/data/znl/gom/data/tick/ru1701-20161118.tick", &data, &info));
//
//     std::thread t(thread_write_tick, data, info);
//     std::this_thread::sleep_for(std::chrono::seconds(10));
//
//     SharedMemory* memReader = SharedMemory::getMemMapping();
//     EXPECT_EQ(0, memReader->OpenRead("test dataset memory", DATASET(CtpMarketData)::get_memory_size(info->di.maxsize)));
//     EXPECT_NE(nullptr, memReader->GetDataPtr());
//
//     auto reader = new DATASET(CtpMarketData)("CtpMarketData", 'r');
//     EXPECT_NE(nullptr, reader);
//     reader->set_memory(memReader->GetDataPtr(), nullptr, info->di.maxsize, 'r');
//     reader->set_newdata_cb(new_tick);
//
//     while (reader->got_new_data()
//            && reader->get_data_amount() < (int64_t)info->di.maxsize) {
//       std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
//
//     t.join();
//
//     SafeDeletePtr(reader);
//
//     memReader->CloseFile();
//     SafeDeletePtr(memReader);
//
//
//     SafeDeleteAry(data);
//     SafeDeletePtr(info);
// }
//
//
// //
// TEST(TsDataFrameMemory, read_write) {
//     CTPMD* data = 0;
//     CTPMDInfo* info = 0;
//
//     // EXPECT_EQ(0, load_data_from_file("/work/sean/projects/build/data/ru1701-20161118.mmd", &data, &info));
//     EXPECT_EQ(0, load_data_from_file("/data/znl/gom/data/tick/ru1701-20161118.mmd", &data, &info));
//
//     SharedMemory* memWriter = SharedMemory::getMemMapping();
//     EXPECT_EQ(0, memWriter->OpenWrite("/tmp", "test dataset memory", TSDATAFRAME(CTPMD)::get_memory_size(info->di.maxsize)));
//     EXPECT_NE(nullptr, memWriter->GetDataPtr());
//
//     SharedMemory* memReader = SharedMemory::getMemMapping();
//     EXPECT_EQ(0, memReader->OpenRead("test dataset memory", TSDATAFRAME(CtpMarketData)::get_memory_size(info->di.maxsize)));
//     EXPECT_NE(nullptr, memReader->GetDataPtr());
//
//     auto writer = new TSDATAFRAME(CTPMD)("CTPMD", 'w');
//     auto reader = new TSDATAFRAME(CTPMD)("CTPMD", 'r');
//     EXPECT_NE(nullptr, writer);
//     EXPECT_NE(nullptr, reader);
//
//     writer->set_memory(memWriter->GetDataPtr(), nullptr, info->di.maxsize, 'w');
//     for (size_t i = 0; i < info->di.maxsize; ++i) {
//       writer->append(data + i);
//     }
//
//     for (size_t i = 0; i < info->di.maxsize; ++i) {
//       EXPECT_EQ(0, memcmp(data + i, writer->at(i), sizeof(CTPMD)));
//       for (size_t j = 0; j < writer->get_clo_amount(); ++j)
//         EXPECT_EQ(writer->get_data(i, j), *((double*)(data + i) + j));
//     }
//
//     reader->set_memory(memReader->GetDataPtr(), nullptr, info->di.maxsize, 'r');
//     for (size_t i = 0; i < info->di.maxsize; ++i) {
//       EXPECT_EQ(0, memcmp(data + i, reader->at(i), sizeof(CTPMD)));
//       for (size_t j = 0; j < reader->get_clo_amount(); ++j)
//         EXPECT_EQ(reader->get_data(i, j), *((double*)(data + i) + j));
//     }
//
//     SafeDeletePtr(reader);
//     SafeDeletePtr(writer);
//
//     memReader->CloseFile();
//     SafeDeletePtr(memReader);
//
//     memWriter->CloseFile();
//     SafeDeletePtr(memWriter);
//
//     SafeDeleteAry(data);
//     SafeDeletePtr(info);
// }
//
//
//
// static void thread_write_ctpmd(CTPMD* data, CTPMDInfo* info) {
//     SharedMemory* memWriter = SharedMemory::getMemMapping();
//     EXPECT_EQ(0, memWriter->OpenWrite("/tmp", "test dataset memory", TSDATAFRAME(CTPMD)::get_memory_size(info->di.maxsize)));
//     EXPECT_NE(nullptr, memWriter->GetDataPtr());
//
//     auto writer = new TSDATAFRAME(CTPMD)("CTPMD", 'w');
//     EXPECT_NE(nullptr, writer);
//
//     std::this_thread::sleep_for(std::chrono::seconds(10));
//
//     writer->set_memory(memWriter->GetDataPtr(), nullptr, info->di.maxsize, 'w');
//     for (size_t i = 0; i < info->di.maxsize; ++i) {
//       writer->append(data + i);
//     }
//
//     SafeDeletePtr(writer);
//     memWriter->CloseFile();
//     SafeDeletePtr(memWriter);
//
// }
//
// static int new_ctpmd(void* dataset, void* data, int64_t pos) {
//   // auto obj = (DATASET(CtpMarketData)*) dataset;
//   // cout << pos << " , " << (CtpMarketData*)data << ", " << obj->info->di.pos << " vs " << obj->nrow << endl;
//   return 0;
// }
//
//
//
// TEST(DataFrameMemory, read_write_ctpmd_thread) {
//     CTPMD* data = 0;
//     CTPMDInfo* info = 0;
//     // EXPECT_EQ(0, load_data_from_file("/work/sean/projects/build/data/ru1701-20161118.mmd", &data, &info));
//     EXPECT_EQ(0, load_data_from_file("/data/znl/gom/data/tick/ru1701-20161118.mmd", &data, &info));
//
//     std::thread t(thread_write_ctpmd, data, info);
//     std::this_thread::sleep_for(std::chrono::seconds(10));
//
//     SharedMemory* memReader = SharedMemory::getMemMapping();
//     EXPECT_EQ(0, memReader->OpenRead("test dataset memory", TSDATAFRAME(CTPMD)::get_memory_size(info->di.maxsize)));
//     EXPECT_NE(nullptr, memReader->GetDataPtr());
//
//     auto reader = new TSDATAFRAME(CTPMD)("CTPMD", 'r');
//     EXPECT_NE(nullptr, reader);
//     reader->set_memory(memReader->GetDataPtr(), nullptr, info->di.maxsize, 'r');
//     reader->set_newdata_cb(new_ctpmd);
//
//     while (reader->got_new_data()
//            && reader->get_data_amount() < (int64_t)info->di.maxsize) {
//       std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
//
//     t.join();
//
//     SafeDeletePtr(reader);
//
//     memReader->CloseFile();
//     SafeDeletePtr(memReader);
//
//
//     SafeDeleteAry(data);
//     SafeDeletePtr(info);
// }
//
//
// TEST(ClassDataSetMemory, read_write) {
//   auto dsmWriter = new DataSetMemory<CtpMarketData, CtpMarketDataInfo>("CtpMarketData", 'w');
//   auto dsmReader = new DataSetMemory<CtpMarketData, CtpMarketDataInfo>("CtpMarketData", 'r');
//
//   delete dsmReader;
//   delete dsmWriter;
// }
//

//
// static CtpMdWriter *_writer_ = nullptr;
//
// int new_data(CThostFtdcDepthMarketDataField *pDepthMarketData) {
//   _writer_->addNewTick(pDepthMarketData);
//   return 0;
// }
//
// TEST(CtpMdWriter, ctpmd_memory) {
//   EXPECT_EQ(0, initCtpMdIf(confpath));
//   CtpMdWriter* mdWriter = new CtpMdWriter();
//   EXPECT_EQ(0, mdWriter->initMemory());
//
//   _writer_ = mdWriter;
//
//   gmdspi = new CtpMdSpi;
//   gmdspi->set_data_cb(new_data);
//   std::thread t(thread_connect_to_ctpmd);
//   std::this_thread::sleep_for(std::chrono::seconds(60));
//
//   gmdspi->unsub_instruments();
//   std::this_thread::sleep_for(std::chrono::seconds(6));
//
//   gmdspi->user_logout();
//   std::this_thread::sleep_for(std::chrono::seconds(6));
//
//   gmdspi->Release();
//   std::this_thread::sleep_for(std::chrono::seconds(6));
//
//   t.detach();
//
//   SafeDeletePtr(gmdspi);
//   EXPECT_EQ(0, mdWriter->destroyMemory());
//   SafeDeletePtr(mdWriter);
//   releaseCtpMdIf();
//
// }
//
// int g_run = 1;
//
// int ok(DATAPTR* dataptr, int64_t* spos, int64_t* epos) {
//   printf("ok\n");
//   return 0;
// }
//
// TEST(CtpMdWriter, reader) {
//   EXPECT_EQ(0, initCtpMdIf(confpath));
//   CtpMdWriter* mdWriter = new CtpMdWriter();
//   EXPECT_EQ(0, mdWriter->initMemory());
//
//   DATASET(CtpMarketData) * cmd = mdWriter->getTickbuf(0);
//   cmd->set_newdata_cb(ok);
//
//   std::thread t(start_data_reader<DataSet, CtpMarketData, CtpMarketDataInfo>,
//   cmd, 1);
//
//   std::this_thread::sleep_for(std::chrono::seconds(60));
//   g_run = 0;
//
//   t.join();
//
//   EXPECT_EQ(0, mdWriter->destroyMemory());
//   SafeDeletePtr(mdWriter);
//   releaseCtpMdIf();
// }
//

/* DataFrame
TEST(Data, DataFrame) {

  auto td = new TSDATAFRAME(CtpMarketData)();

  td->ncol = 0;
  td->nrow = 0;

  delete td;

  auto  td2 = new TSDATAFRAME(Candle)(123);
  CandleInfo info;
  td2->init(100, &info);

  size_t t1 = sizeof(Candle) * 200 + sizeof(CandleInfo);
  size_t t2 = TSDATAFRAME(Candle)::get_memory_size(td2->maxsize);
  EXPECT_EQ(t1, t2);

  char* mem = new char[t2];
  td2->set_memory(mem, &info, 100);

  td2->free_data_set();

  delete[] mem;
  delete td2;
}

TEST(DataFrame, construct) {
  const size_t max_size = 100;
  CandleInfo cdi;
  memset(&cdi, 0, sizeof(CandleInfo));

  size_t memory_size = TSDATAFRAME(Candle)::get_memory_size(max_size);
  EXPECT_EQ(memory_size, sizeof(Candle) * max_size * 2 + sizeof(CandleInfo));
  char* memptr = new char[memory_size];

  cdi.di.cbsize = sizeof(Candle);
  cdi.di.pos = - 1;
  cdi.di.maxsize = 100;
  strcpy(cdi.instrument, "rb1701");
  cdi.period = CANDLE_PERIOD_TYPE_D1;
  cdi.di.data = memptr + sizeof(CandleInfo);

  TSDATAFRAME(Candle) dfCandle;
  dfCandle.set_memory(memptr, &cdi, max_size);

  EXPECT_EQ(dfCandle.cbsize, sizeof(Candle));
  EXPECT_EQ(dfCandle.infosize, sizeof(CandleInfo));
  EXPECT_EQ(dfCandle.data, cdi.di.data);

  EXPECT_EQ(dfCandle.maxsize, max_size);
  EXPECT_EQ(dfCandle.ncol, 7);
  EXPECT_EQ(dfCandle.nrow, 0);
  EXPECT_EQ(dfCandle.myown_memory, false);

  EXPECT_EQ((char*)dfCandle.info, memptr);
  EXPECT_EQ((char*)dfCandle.key, memptr + sizeof(Candle) * max_size + sizeof(CandleInfo));
  EXPECT_EQ((char*)dfCandle.series[0], memptr + sizeof(Candle) * max_size + sizeof(CandleInfo));
  EXPECT_EQ(dfCandle.series[1], dfCandle.series[0] + max_size);
  EXPECT_EQ(dfCandle.series[2], dfCandle.series[1] + max_size);
  EXPECT_EQ(dfCandle.series[3], dfCandle.series[2] + max_size);
  EXPECT_EQ(dfCandle.series[4], dfCandle.series[3] + max_size);
  EXPECT_EQ(dfCandle.series[5], dfCandle.series[4] + max_size);
  EXPECT_EQ(dfCandle.series[6], dfCandle.series[5] + max_size);

  int k = std::memcmp(dfCandle.info, &cdi, sizeof(CandleInfo));
  EXPECT_EQ(0, k);

  delete [] memptr;
}

TEST(DataFrame, append) {
    const size_t max_size = 100;

    CandleInfo cdi;
    memset(&cdi, 0, sizeof(Candle));

    size_t memory_size = TSDATAFRAME(Candle)::get_memory_size(max_size);
    EXPECT_EQ(memory_size, sizeof(Candle) * max_size * 2 + sizeof(CandleInfo));
    char* memptr = new char[memory_size];

    TSDATAFRAME(Candle) dfCandle;
    cdi.di.cbsize = sizeof(Candle);
    cdi.di.pos = - 1;
    cdi.di.maxsize = 100;
    strcpy(cdi.instrument, "rb1701");
    cdi.period = CANDLE_PERIOD_TYPE_D1;
    cdi.di.data = memptr + sizeof(CandleInfo);

    dfCandle.set_memory(memptr, &cdi, max_size);
    double k = 0;
    for (size_t i = 0; i < max_size; ++i) {
      Candle c;
      c.key = k++;
      c.open = k++;
      c.high = k++;
      c.low = k++;
      c.close = k++;
      c.volume = k++;
      c.interest = k++;
      dfCandle.append(&c);
    }

    Candle* c = (Candle*)(memptr + sizeof(CandleInfo));
    k = 0;
    for (size_t i = 0; i < max_size; ++i) {
      EXPECT_EQ((size_t)c[i].key , k++);
      EXPECT_EQ((size_t)c[i].open, k++);
      EXPECT_EQ((size_t)c[i].high, k++);
      EXPECT_EQ((size_t)c[i].low , k++);
      EXPECT_EQ((size_t)c[i].close, k++);
      EXPECT_EQ((size_t)c[i].volume, k++);
      EXPECT_EQ((size_t)c[i].interest, k++);
    }

    double* series = (double*)(memptr + max_size * sizeof(Candle) + sizeof(CandleInfo));
    for (size_t i = 0; i < sizeof(Candle) / sizeof(double); ++i) {
       EXPECT_EQ(series, dfCandle.series[i]);
       for (size_t j = 0; j < max_size; ++j)
         EXPECT_EQ((size_t)series[j], j * 7 + i);
       series += max_size;
     }

    VecColName cols = {
        "key",
        "open", "high", "low", "close",
        "volume", "interest",
    };
    dfCandle.set_columns(cols);

    EXPECT_EQ(dfCandle["key"],      dfCandle.series[0]);
    for (size_t j = 0; j < max_size; ++j)
      EXPECT_EQ(dfCandle["key"][j], j * 7);
    EXPECT_EQ(dfCandle["open"],     dfCandle.series[1]);
    for (size_t j = 0; j < max_size; ++j)
      EXPECT_EQ(dfCandle["open"][j], j * 7 + 1);
    EXPECT_EQ(dfCandle["high"],     dfCandle.series[2]);
    for (size_t j = 0; j < max_size; ++j)
      EXPECT_EQ(dfCandle["high"][j], j * 7 + 2);
    EXPECT_EQ(dfCandle["low"],      dfCandle.series[3]);
    for (size_t j = 0; j < max_size; ++j)
      EXPECT_EQ(dfCandle["low"][j], j * 7 + 3);
    EXPECT_EQ(dfCandle["close"],    dfCandle.series[4]);
    for (size_t j = 0; j < max_size; ++j)
      EXPECT_EQ(dfCandle["close"][j], j * 7 + 4);
    EXPECT_EQ(dfCandle["volume"],   dfCandle.series[5]);
    for (size_t j = 0; j < max_size; ++j)
      EXPECT_EQ(dfCandle["volume"][j], j * 7 + 5);
    EXPECT_EQ(dfCandle["interest"], dfCandle.series[6]);
    for (size_t j = 0; j < max_size; ++j)
      EXPECT_EQ(dfCandle["interest"][j], j * 7 + 6);

    dfCandle.free_data_set();
    delete [] memptr;
}



TEST(DataFrame, sample) {
    const size_t max_size = 100;

    CandleInfo cdi;
    memset(&cdi, 0, sizeof(Candle));

    size_t memory_size = TSDATAFRAME(Candle)::get_memory_size(max_size);
    EXPECT_EQ(memory_size, sizeof(Candle) * max_size * 2 + sizeof(CandleInfo));
    char* memptr = new char[memory_size];

    TSDATAFRAME(Candle) dfCandle;
    cdi.di.cbsize = sizeof(Candle);
    cdi.di.pos = - 1;
    cdi.di.maxsize = 100;
    strcpy(cdi.instrument, "rb1701");
    cdi.period = CANDLE_PERIOD_TYPE_D1;
    cdi.di.data = memptr + sizeof(CandleInfo);

    dfCandle.set_memory(memptr, &cdi, max_size);
    VecColName cols = {
        "key",
        "open", "high", "low", "close",
        "volume", "interest",
    };
    dfCandle.set_columns(cols);

    double k = 0;
    for (size_t i = 0; i < max_size; ++i) {
      Candle c;
      c.key = k++;
      c.open = k++;
      c.high = k++;
      c.low = k++;
      c.close = k++;
      c.volume = k++;
      c.interest = k++;
      dfCandle.append(&c);
    }

    size_t interval = 20, offset = 3;
    auto dfSampled = dfCandle.sample(interval, offset);
    TSDATAFRAME(Candle)& df = *dfSampled;
    df.set_columns(cols);

    for (size_t i = offset, k = 0; i < max_size; i += interval, ++k) {
      EXPECT_EQ(dfCandle[i], df[k]);
      EXPECT_EQ(dfCandle[i].open, df["open"][k]);
      EXPECT_EQ(dfCandle[i].high, df["high"][k]);
      EXPECT_EQ(dfCandle[i].low, df["low"][k]);
      EXPECT_EQ(dfCandle[i].close, df["close"][k]);
      EXPECT_EQ(dfCandle[i].volume, df["volume"][k]);
    }

    interval = 2, offset = 0;

    auto dfSampled2 = dfCandle.sample(interval, offset);
    TSDATAFRAME(Candle)& df2 = *dfSampled2;
        df2.set_columns(cols);

        for (size_t i = offset, k = 0; i < max_size; i += interval, ++k) {
          EXPECT_EQ(dfCandle[i], df2[k]);
          EXPECT_EQ(dfCandle[i].open, df2["open"][k]);
          EXPECT_EQ(dfCandle[i].high, df2["high"][k]);
          EXPECT_EQ(dfCandle[i].low, df2["low"][k]);
          EXPECT_EQ(dfCandle[i].close, df2["close"][k]);
          EXPECT_EQ(dfCandle[i].volume, df2["volume"][k]);
        }

    dfCandle.free_data_set();
    delete [] memptr;
}
*/

/* Udp transfer
void udp_server(UdpDataTransfer *server) {
  server->start_server();
}

TEST(UdpDataTransfer, server_client) {
  UdpDataTransfer server("10.10.255.255", 31111);
  std::thread t(udp_server, &server);
  UdpDataTransfer client("10.10.255.255", 31111);
  client.init_client();
  for (int i = 0; i < 30; ++i) {
    client.send((void*)"Hello", 5);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  server.stop_server();
  t.join();
}
*/

#include "udp_server.h"

void start_udp_server(UdpSrvReceiver* rev) {
  rev->start();
}

int test_process_udp_package(const char* buf, int nread, const char* sender) {
  fprintf(stdout, "Recv from %s\n", sender);
  fwrite(buf, sizeof(char), nread, stdout);
  fprintf(stdout, "\n");
  fflush(stdout);

  return 0;
}


TEST(uv_udp, server) {
  UdpSrvReceiver * rev = new UdpSrvReceiver();
  rev->set_process_callback(test_process_udp_package);
  std::thread t(start_udp_server, rev);
  std::this_thread::sleep_for(std::chrono::seconds(30));
  rev->stop();
  t.detach();
  std::this_thread::sleep_for(std::chrono::seconds(3));
  delete rev;
}
