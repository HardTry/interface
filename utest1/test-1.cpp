#include "ctpif_global.h"
#include "mderr.h"
#include "str.h"
#include "utility.h"

#include "ctpmdspi.h"
#include "ctptdspi.h"

#include "ctpif_data.h"
#include "ctpif.h"

#include "mem.h"

#include "rapidjson/document.h"
#include <chrono>
#include <gtest/gtest.h>
#include <stdio.h>
#include <thread>

// static char confpath[MAX_PATH];
// static char confpath2[MAX_PATH];

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

// static char strJson[] =
//     "{\n"
//     "  \"data-interface\": [\n"
//     "    {\n"
//     "      \"name\": \"ctp\",\n"
//     "      \"param\": {\n"
//     "        \"ctpmd-logpath\": \"d:\\\\projects\\\\build-md\\\\logs\\\\\",\n"
//     "        \"brokerid\": \"9999\",\n"
//     "        \"clientid\": \"068576\",\n"
//     "        \"password\": \"simnow\",\n"
//     "        \"udp\": false,\n"
//     "        \"servers\": [\n"
//     "           \"tcp://180.168.146.187:10010\",\n"
//     "           \"tcp://180.168.146.187:10011\"\n"
//     "        ]\n"
//     "      }\n"
//     "    }\n"
//     "  ],\n"
//     "  \"instrument\" : [\n"
//     "    \"rb1609\",\n"
//     "    \"rb1610\",\n"
//     "    \"m1609\",\n"
//     "    \"SR609\"\n"
//     "  ],\n"
//     "  \"gmd-param\": {\n"
//     "    \"log-path\": \"d:\\\\projects\\\\build-md\\\\logs\\\\gmd.log\"\n"
//     "  }\n"
//     "}\n";
//
// TEST(ctp_json_config, config_is_null) {
//   GmdParam* param = initConfig(0);
//   EXPECT_EQ(param, nullptr);
//   // printf("%s\n", getErrorString(result));
// }
//
// TEST(ctp_json_config, config_is_wrong) {
//   GmdParam* param = initConfig("xxx");
//   EXPECT_EQ(param, nullptr);
// }
//
// TEST(ctp_json_config, good_json_config) {
//   rapidjson::StringStream ss(strJson);
//   rapidjson::Document jsondoc;
//   jsondoc.ParseStream(ss);
//   if (jsondoc.HasParseError()) {
//     cout << "Error at " << jsondoc.GetErrorOffset() << endl;
//     cout << strJson + jsondoc.GetErrorOffset() << endl;
//   }
//
//   EXPECT_EQ(jsondoc.HasParseError(), false);
// }
//
// TEST(ctp_json_config, parse_json_config) {
// #ifdef _WINDOWS
//   int result = initCtpMdIf(confpath);
// #else
//   GmdParam* param = initConfig(confpath);
// #endif
//
//   EXPECT_NE(param, nullptr);
//   EXPECT_NE(param->ctp_param, nullptr);
//   EXPECT_EQ(param->ctp_param->brokerid, "9999");
//   EXPECT_EQ(param->ctp_param->clientid == "068576", true);
//   EXPECT_EQ(param->ctp_param->password, "simnow");
//   EXPECT_EQ(param->ctp_param->udp, false);
//
//   EXPECT_EQ(param->ctp_param->mduri.size(), 3);
//   EXPECT_EQ(param->ctp_param->mduri[0], "tcp://180.168.146.187:10010");
//   EXPECT_EQ(param->ctp_param->mduri[1], "tcp://180.168.146.187:10011");
//
//   EXPECT_EQ(param->ctp_param->tduri.size(), 3);
//   EXPECT_EQ(param->ctp_param->tduri[0], "tcp://180.168.146.187:10000");
//   EXPECT_EQ(param->ctp_param->tduri[1], "tcp://180.168.146.187:10001");
//
//   // EXPECT_EQ(param->ctp_param->inst_file,
//   // "/work/sean/projects/md/md/instruments.json");
//   EXPECT_EQ(param->ctp_param->instrument.size(), 12);
//   // EXPECT_EQ(param->ctp_param->instrument[0], "rb1609");
//   // EXPECT_EQ(param->ctp_param->instrument[1], "rb1610");
//   // EXPECT_EQ(param->ctp_param->instrument[2], "m1609");
//   // EXPECT_EQ(param->ctp_param->instrument[3], "SR609");
//   // EXPECT_EQ(param->ctp_param->instrument[4], "IF1609");
//   // EXPECT_EQ(param->ctp_param->instrument[5], "au1612");
//   // EXPECT_EQ(param->ctp_param->instrument[6], "ag1612");
//
//   //#ifdef _WINDOWS
//   //  EXPECT_EQ(ctp_param->ctpmd_logpath, "d:\\projects\\build\\md\\logs\\");
//   //  EXPECT_EQ(strcmp(gmd_param->gmdlog,
//   //  "d:\\projects\\build\\md\\logs\\gmd.log"),
//   //            0);
//   //#else
//   //  EXPECT_EQ(ctp_param->ctpmd_logpath, "/work/sean/projects/build-md/log");
//   //  EXPECT_EQ(
//   //      strcmp(gmd_param->gmdlog, "/work/sean/projects/build-md/log/gmd.log"),
//   //      0);
//   //#endif
//   EXPECT_EQ(param->exit_time, "16:00:00");
//
//   releaseConfig(param);
// }
//
// TEST(ctp_json_config, config_all_instruments) {
// #ifdef _WINDOWS
//   int result = initCtpMdIf(confpath2);
// #else
//   GmdParam* param = initConfig(confpath2);
// #endif
//
//   EXPECT_NE(param, nullptr);
//   EXPECT_NE(param->ctp_param, (CtpParam *)0);
//   EXPECT_EQ(param->ctp_param->brokerid, "9999");
//   EXPECT_EQ(param->ctp_param->clientid == "068576", true);
//   EXPECT_EQ(param->ctp_param->password, "simnow");
//   EXPECT_EQ(param->ctp_param->udp, false);
//
//   EXPECT_EQ(param->ctp_param->mduri.size(), 3);
//   EXPECT_EQ(param->ctp_param->mduri[0], "tcp://180.168.146.187:10010");
//   EXPECT_EQ(param->ctp_param->mduri[1], "tcp://180.168.146.187:10011");
//
//   EXPECT_EQ(param->ctp_param->tduri.size(), 3);
//   EXPECT_EQ(param->ctp_param->tduri[0], "tcp://180.168.146.187:10000");
//   EXPECT_EQ(param->ctp_param->tduri[1], "tcp://180.168.146.187:10001");
//
//   // EXPECT_EQ(ctp_param->inst_file, confpath2);
//   EXPECT_EQ(param->ctp_param->instrument.size(), 476);
//
//   EXPECT_NE(param, nullptr);
//   EXPECT_EQ(param->exit_time, "16:00:00");
//
//   releaseConfig(param);
// }
//
// TEST(MemMapping, open_write) {
//   const int size__ = 1024;
//   SharedMemory *sm = SharedMemory::getMemMapping();
//   int result = sm->OpenWrite("", "mydata", size__);
//   EXPECT_EQ(result, 0);
//   EXPECT_EQ(sm->IsValidFile(), true);
//
//   // test write
//   char *ptr = (char *)sm->GetDataPtr();
//   int i = 0;
//   for (; i < size__; ++i, ++ptr)
//     *ptr = 'z';
//
//   // test read
//   ptr = (char *)sm->GetDataPtr();
//   for (; i < size__; ++i, ++ptr)
//     EXPECT_EQ(*ptr, 'z');
//
//   sm->CloseFile();
//   sm->Delete();
// }
//
// TEST(MemMapping, open_write_read) {
//   const int size__ = 1024;
//   SharedMemory *smWrite = SharedMemory::getMemMapping();
//   int result = smWrite->OpenWrite("", "testmem", size__);
//   EXPECT_EQ(result, 0);
//   EXPECT_EQ(smWrite->IsValidFile(), true);
//
//   // test write
//   char *ptr = (char *)smWrite->GetDataPtr();
//   int i = 0;
//   for (; i < size__; ++i, ++ptr)
//     *ptr = 'z';
//
//   // test read
//   ptr = (char *)smWrite->GetDataPtr();
//   for (i = 0; i < size__; ++i, ++ptr)
//     EXPECT_EQ(*ptr, 'z');
//
//   SharedMemory *smRead = SharedMemory::getMemMapping();
//   result = smRead->OpenRead("testmem", size__);
//   EXPECT_EQ(result, 0);
//   EXPECT_EQ(smRead->IsValidFile(), true);
//
//   // test read
//   ptr = (char *)smRead->GetDataPtr();
//   for (i = 0; i < size__; ++i, ++ptr)
//     EXPECT_EQ(*ptr, 'z');
//
//   smRead->CloseFile();
//   smRead->Delete();
//
//   smWrite->CloseFile();
//   smWrite->Delete();
// }
//
//
// TEST(ctp_market_data, convert_ctpmd_2_ctpmmd) {
//   CtpMarketData data;
//   char day[] = "20160905";
//   char tt[] = "11:23:36";
//   time_t t = make_the_time(day, tt);
//   CTPMMD mmd;
//   memset(&data, 'a', sizeof(CtpMarketData));
//   strcpy_s(data.TradingDay, sizeof(data.TradingDay), day);
//   strcpy_s(data.UpdateTime, sizeof(data.UpdateTime), tt);
//   memset(&mmd, 'a', sizeof(CTPMMD));
//   mmd.UpdateTime = t;
//
//   CTPMMD m;
//   convert_ctpmd_2_ctpmmd(&m, &data);
//   EXPECT_EQ(m.AskPrice1 == mmd.AskPrice1, true);
//   EXPECT_EQ(m.AskVolume1 == mmd.AskVolume1, true);
//   EXPECT_EQ(m.BidPrice1 == mmd.BidPrice1, true);
//   EXPECT_EQ(m.BidVolume1 == mmd.BidVolume1, true);
//   EXPECT_EQ(m.LastPrice == mmd.LastPrice, true);
//   EXPECT_EQ(m.OpenInterest == mmd.OpenInterest, true);
//   // EXPECT_EQ(m.Turnover == mmd.Turnover, true);
//   EXPECT_EQ(m.UpdateMillisec == mmd.UpdateMillisec, true);
//   EXPECT_EQ(m.UpdateTime == mmd.UpdateTime, true);
//   EXPECT_EQ(m.Volume == mmd.Volume, true);
// }
//
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


int main(int argc, char **argv) {

#ifdef _WINDOWS
#ifdef _DEBUG_
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  _CrtDumpMemoryLeaks();
#endif //_DEBUG_
#endif //_WINDOWS

  // if (argc != 3) {
  //   printf("Usage testing <json config file> <json config file2>\n");
  //   return (0);
  // }
  //
  // strcpy_s(confpath, MAX_PATH, argv[1]);
  // strcpy_s(confpath2, MAX_PATH, argv[2]);
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
