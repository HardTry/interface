#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <regex>
#include <stdlib.h>
#include <string>
#include <vector>
#include <time.h>
#include <thread>
#include <stdio.h>
#include "gom_global.h"
#include "str.h"
#include "utility.h"
#include "mem.h"

using namespace std;

// static char confpath[MAX_PATH];
/*****************************************************************************************
static double epsilon = 1e-10;

TEST(regex_test, example_1) {
  std::vector<regex> regexInstruments;
  regexInstruments.push_back(regex("rb[[:digit:]]+"));
  regexInstruments.push_back(regex("j[[:digit:]]+"));
  regexInstruments.push_back(regex("i[[:digit:]]+"));
  regexInstruments.push_back(regex("CF[[:digit:]]+"));
  regexInstruments.push_back(regex("SR[[:digit:]]+"));
  regexInstruments.push_back(regex("ni[[:digit:]]+"));
  regexInstruments.push_back(regex("au[[:digit:]]+"));
  regexInstruments.push_back(regex("ag[[:digit:]]+"));
  regexInstruments.push_back(regex("ru[[:digit:]]+"));
  regexInstruments.push_back(regex("p1[[:digit:]]+"));
  regexInstruments.push_back(regex("m1[[:digit:]]+"));
  regexInstruments.push_back(regex("IF[[:digit:]]+"));

  EXPECT_EQ(match_index(regexInstruments, "rb1701"), 0);
  EXPECT_EQ(match_index(regexInstruments, "j1701"), 1);
  EXPECT_EQ(match_index(regexInstruments, "i1701"), 2);
  EXPECT_EQ(match_index(regexInstruments, "CF701"), 3);
  EXPECT_EQ(match_index(regexInstruments, "SR701"), 4);
  EXPECT_EQ(match_index(regexInstruments, "ni1701"), 5);
  EXPECT_EQ(match_index(regexInstruments, "au1612"), 6);
  EXPECT_EQ(match_index(regexInstruments, "ag1612"), 7);
  EXPECT_EQ(match_index(regexInstruments, "ru1701"), 8);
  EXPECT_EQ(match_index(regexInstruments, "p1701"), 9);
  EXPECT_EQ(match_index(regexInstruments, "m1701"), 10);
  EXPECT_EQ(match_index(regexInstruments, "IF1610"), 11);
}


TEST(multi_map, example_1) {
  std::multimap<std::string, std::string> mapInst;

  EXPECT_EQ(getInstCode("rb1701"), "rb");
  EXPECT_EQ(getInstCode("rb1706"), "rb");
  EXPECT_EQ(getInstCode("rb1709"), "rb");
  EXPECT_EQ(getInstCode("rb1712"), "rb");
  EXPECT_EQ(getInstCode("j1701"), "j");
  EXPECT_EQ(getInstCode("j1703"), "j");
  EXPECT_EQ(getInstCode("j1706"), "j");
  EXPECT_EQ(getInstCode("j1710"), "j");
  EXPECT_EQ(getInstCode("i1701"), "i");
  EXPECT_EQ(getInstCode("i1703"), "i");
  EXPECT_EQ(getInstCode("i1706"), "i");
  EXPECT_EQ(getInstCode("i1710"), "i");
  EXPECT_EQ(getInstCode("i1801"), "i");
  EXPECT_EQ(getInstCode("i1802"), "i");
  EXPECT_EQ(getInstCode("i1803"), "i");

  mapInst.insert(std::make_pair(getInstCode("rb1701"), "rb1701"));
  mapInst.insert(std::make_pair(getInstCode("rb1706"), "rb1706"));
  mapInst.insert(std::make_pair(getInstCode("rb1709"), "rb1709"));
  mapInst.insert(std::make_pair(getInstCode("rb1712"), "rb1712"));
  mapInst.insert(std::make_pair(getInstCode("j1701"), "j1701"));
  mapInst.insert(std::make_pair(getInstCode("j1703"), "j1703"));
  mapInst.insert(std::make_pair(getInstCode("j1706"), "j1706"));
  mapInst.insert(std::make_pair(getInstCode("j1710"), "j1710"));
  mapInst.insert(std::make_pair(getInstCode("i1701"), "i1701"));
  mapInst.insert(std::make_pair(getInstCode("i1703"), "i1703"));
  mapInst.insert(std::make_pair(getInstCode("i1706"), "i1706"));
  mapInst.insert(std::make_pair(getInstCode("i1710"), "i1710"));
  mapInst.insert(std::make_pair(getInstCode("i1801"), "i1801"));
  mapInst.insert(std::make_pair(getInstCode("i1802"), "i1803"));
  mapInst.insert(std::make_pair(getInstCode("i1803"), "i1804"));

  EXPECT_EQ(mapInst.count("rb"), 4);
  EXPECT_EQ(mapInst.count("j"), 4);
  EXPECT_EQ(mapInst.count("i"), 7);
  EXPECT_EQ(mapInst.count("m"), 0);

  // pair<multimap<string, string>::iterator, multimap<string,
  // string>::iterator> ppp;
  EXPECT_EQ(minString(mapInst, "rb"), "rb1701");
  EXPECT_EQ(minString(mapInst, "j"), "j1701");
  EXPECT_EQ(minString(mapInst, "i"), "i1701");
  EXPECT_EQ(minString(mapInst, "m"), "");
}



TEST(exit_time, exit_time) {
  struct tm ttm;
  char buf[20];

  for (int interval = 0; interval <= 1000; interval++) {
    time_t now = time(0);
    time_t t0 = 0;
    localtime_s(&ttm, &now);
    ttm.tm_hour = ttm.tm_min = ttm.tm_sec = 0;
    t0 = mktime(&ttm);

    int i = rand() % 3590;
    time_t tt = t0 + i;
    localtime_s(&ttm, &tt);
    snprintf(buf, 20, "%d:%d:%d", ttm.tm_hour, ttm.tm_min, ttm.tm_sec);
    time_t today_exit = get_exit_time(buf);
    EXPECT_EQ(today_exit - t0, i);
  }
}

TEST(tb_2_gmd, tb_2_ctpmmd) {
#ifdef _WINDOWS
  char in[] = "D:\\projects\\md\\utest2\\rb888_Tick_test.csv";
#else
  char in[] = "/work/sean/projects/md/utest2/rb888_Tick_test.csv";
#endif
  char out[] = "rb888_test.mmd";

  int amount = tb_2_ctpmmd(in, out);
  EXPECT_GT(amount, 0);

  FILE* fpOut = 0;
  EXPECT_EQ(0, fopen_s(&fpOut, out, "rb"));
  fseek(fpOut, 0, SEEK_END);
  int size = ftell(fpOut);
  EXPECT_EQ(amount * sizeof(CTPMMD), size);
  fseek(fpOut, 0, SEEK_SET);

  FILE* fpIn = 0;
  EXPECT_EQ(0, fopen_s(&fpIn, in, "r"));
  char buf[1024];
  int lines = 0;

  CTPMMD mmd;
  CTPMMD diskmmd;
  char date[32];
  char time[32];
  int ignore1, ignore2;

  while(fgets(buf, 1024, fpIn)) {
    if ((*buf == 0) || (*buf == '#') || (*buf == '\r') || (*buf == '\n'))
      continue;

    sscanf(buf, "%[^,],0.%[^,],%d,%d,%d,%d,%d,%lf,%lf,%lf,%lf",
           date, time, &ignore1,
           &mmd.Volume, &ignore2, &mmd.AskVolume1, &mmd.BidVolume1,
           &mmd.LastPrice,
           &mmd.AskPrice1,
           &mmd.BidPrice1,
           &mmd.OpenInterest);

    uint32_t t = atoi(time) * (strlen(time) == 6 ? 1000 : 100);
    mmd.UpdateTime = make_the_time(date, t / 1000);
    mmd.UpdateMillisec = t % 1000;

    EXPECT_EQ(1, fread(&diskmmd, sizeof(CTPMMD), 1, fpOut));
    EXPECT_EQ(0, memcmp(&mmd, &diskmmd, sizeof(CTPMMD)));

    ++lines;
  }


  fclose(fpOut);
  fclose(fpIn);

  EXPECT_EQ(lines, amount);
}

TEST(tb_2_gmd, tb_2_min) {
#ifdef _WINDOWS
  char in[] = "D:\\projects\\md\\utest2\\ru888_1m_test.csv";
#else
  char in[] = "/work/sean/projects/md/utest2/ru888_1m_test.csv";
#endif
  char out[] = "/work/sean/projects/md/utest2/ru888.m1";

  size_t amount = 0;
  int ret = tb_2_min("ru888", in, out, &amount);
  EXPECT_EQ(ret, 0);

  FILE* fpOut = 0;
  EXPECT_EQ(0, fopen_s(&fpOut, out, "rb"));
  char buf[1024];
  memset(buf, 0, 1024);
  size_t len = strlen("ru888") + 1 + sizeof(KINDLE_PERIOD_TYPE) + sizeof(size_t);
  EXPECT_EQ(len, fread(buf, sizeof(char), len, fpOut));
  EXPECT_EQ(strcmp("ru888", buf), 0);
  EXPECT_EQ(*(reinterpret_cast<KINDLE_PERIOD_TYPE*>(buf + 6)), KINDLE_PERIOD_TYPE_m1);
  EXPECT_EQ(*(reinterpret_cast<size_t*>(buf + 6 + sizeof(KINDLE_PERIOD_TYPE))), static_cast<size_t>(amount));

  long spos = ftell(fpOut);
  fseek(fpOut, 0, SEEK_END);
  long epos = ftell(fpOut);
  EXPECT_EQ(amount, (int)((epos - spos) / sizeof(KINDLE)));
  fseek(fpOut, spos, SEEK_SET);

  FILE* fpIn = 0;
  EXPECT_EQ(0, fopen_s(&fpIn, in, "r"));
  char date[32];
  char time[32];
  int lines = 0;

  KINDLE kCsv, kOut;

  while(fgets(buf, 1024, fpIn)) {
    if ((*buf == 0) || (*buf == '#') || (*buf == '\r') || (*buf == '\n'))
      continue;

    int n = sscanf(buf, "%[^,],0.%[^,],%lf,%lf,%lf,%lf,%u,%lf", date, time,
           &kCsv.open, &kCsv.high, &kCsv.low, &kCsv.close, &kCsv.vol,
           &kCsv.interest);
    EXPECT_EQ(n, 8);

    uint32_t t = atoi(time);
    size_t len = strlen(time);
    uint32_t p = 0;
    switch (len) {
    case 1:
      p = 100000;
      break;
    case 2:
      p = 10000;
      break;
    case 3:
      p = 1000;
      break;
    case 4:
      p = 100;
      break;
    }

    kCsv.UpdateTime = make_the_time(date, t * p);
    EXPECT_EQ(1, fread(&kOut, sizeof(KINDLE), 1, fpOut));
    EXPECT_LT(kCsv.close - kOut.close, epsilon);
    EXPECT_LT(kCsv.high - kOut.high, epsilon);
    EXPECT_LT(kCsv.interest - kOut.interest, epsilon);
    EXPECT_LT(kCsv.low - kOut.low, epsilon);
    EXPECT_LT(kCsv.open - kOut.open, epsilon);
    EXPECT_EQ(kCsv.UpdateTime, kOut.UpdateTime);
    EXPECT_EQ(kCsv.vol, kOut.vol);

    ++lines;
  }


  fclose(fpOut);
  fclose(fpIn);

  EXPECT_EQ(lines, amount);
}

TEST(py_and_c, hello_world) {
  FILE* pp = popen("python /work/sean/projects/md/utest2/helloworld.py", "r");
  EXPECT_NE(pp, (FILE*)0);

  char buf[100];
  memset(buf, 0, 100);
  fread(buf, sizeof(char), 100, pp);
  EXPECT_EQ(strcmp("Hello World!\n", buf), 0);

  pclose(pp);
}

TEST(py_and_c, shared_read) {
  MemMapping mm;
  const char* filepath = {"/tmp/tttttttttt"};
  FILE* fp = 0;
  EXPECT_EQ(fopen_s(&fp, filepath, "w+b"), 0);
  const char* memname = {"test_py_and_c"};
  const size_t nFileSize = sizeof(CTPMMD) * 10 + sizeof(KINDLE) * 10;
  EXPECT_EQ(mm.OpenWrite(filepath, memname, nFileSize), 0);


  FILE* pp = popen("python /work/sean/projects/md/utest2/shared-read.py", "r");
  EXPECT_NE(pp, (FILE*)0);

  char buf[nFileSize * 100];
  memset(buf, 0, nFileSize * 100);
  fread(buf, sizeof(char), nFileSize * 100, pp);
  EXPECT_EQ(strcmp("CTPMMD OK!\nKINDLE OK!\n", buf), 0);

  pclose(pp);

  EXPECT_EQ(mm.CloseFile(), 0);
  EXPECT_EQ(fclose(fp), 0);
  EXPECT_EQ(remove(filepath), 0);
}

void verify_kindle_ru888(KINDLE* kindle) {
  struct tm ttm;
  memset(&ttm, 0, sizeof(ttm));
  ttm.tm_year = 2008 - 1900;
  ttm.tm_mon = 5 - 1;
  ttm.tm_mday = 28;
  ttm.tm_hour = 11;
  ttm.tm_sec = 0;

  //first 3 lines
  //20080528,0.1103,26515,26530,26490,26500,5620,134146
  //20080528,0.1104,26500,26505,26475,26475,3718,134290
  //20080528,0.1105,26480,26480,26430,26440,5588,134568

  EXPECT_LT(kindle[0].open  - 26515, epsilon);
  EXPECT_LT(kindle[0].high  - 26530, epsilon);
  EXPECT_LT(kindle[0].low   - 26490, epsilon);
  EXPECT_LT(kindle[0].close - 26500, epsilon);
  EXPECT_EQ(kindle[0].vol, 5620);
  EXPECT_LT(kindle[0].interest - 134146, epsilon);
  ttm.tm_min = 3;
  time_t t = mktime(&ttm);
  EXPECT_EQ(kindle[0].UpdateTime, t);

  EXPECT_LT(kindle[1].open  - 26500, epsilon);
  EXPECT_LT(kindle[1].high  - 26505, epsilon);
  EXPECT_LT(kindle[1].low   - 26475, epsilon);
  EXPECT_LT(kindle[1].close - 26475, epsilon);
  EXPECT_EQ(kindle[1].vol, 3718);
  EXPECT_LT(kindle[1].interest - 134290, epsilon);
  ttm.tm_min = 4;
  t = mktime(&ttm);
  EXPECT_EQ(kindle[1].UpdateTime, t);

  EXPECT_LT(kindle[2].open  - 26480, epsilon);
  EXPECT_LT(kindle[2].high  - 26480, epsilon);
  EXPECT_LT(kindle[2].low   - 26430, epsilon);
  EXPECT_LT(kindle[2].close - 26440, epsilon);
  EXPECT_EQ(kindle[2].vol, 5588);
  EXPECT_LT(kindle[2].interest - 134568, epsilon);
  ttm.tm_min = 5;
  t = mktime(&ttm);
  EXPECT_EQ(kindle[2].UpdateTime, t);

  // 1783 -> 20080610,0.1457,25365,25450,25360,25435,7580,110568
  // 1784 -> 20080610,0.1458,25445,25490,25420,25435,7056,109680
  // 1785 -> 20080610,0.1459,25435,25450,25420,25435,4800,108826
  ttm.tm_mon = 6 - 1;
  ttm.tm_mday = 10;
  ttm.tm_hour = 14;
  ttm.tm_min = 57;
  t = mktime(&ttm);
  EXPECT_EQ(kindle[1783].UpdateTime, t);
  EXPECT_LT(kindle[1783].open  - 25365, epsilon);
  EXPECT_LT(kindle[1783].high  - 25450, epsilon);
  EXPECT_LT(kindle[1783].low   - 25360, epsilon);
  EXPECT_LT(kindle[1783].close - 25435, epsilon);
  EXPECT_EQ(kindle[1783].vol, 7580);
  EXPECT_LT(kindle[1783].interest - 110568, epsilon);

  ttm.tm_min = 58;
  t = mktime(&ttm);
  EXPECT_EQ(kindle[1784].UpdateTime, t);
  EXPECT_LT(kindle[1784].open  - 25445, epsilon);
  EXPECT_LT(kindle[1784].high  - 25490, epsilon);
  EXPECT_LT(kindle[1784].low   - 25420, epsilon);
  EXPECT_LT(kindle[1784].close - 25435, epsilon);
  EXPECT_EQ(kindle[1784].vol, 7056);
  EXPECT_LT(kindle[1784].interest - 109680, epsilon);

  ttm.tm_min = 59;
  t = mktime(&ttm);
  EXPECT_EQ(kindle[1785].UpdateTime, t);
  EXPECT_LT(kindle[1785].open  - 25435, epsilon);
  EXPECT_LT(kindle[1785].high  - 25450, epsilon);
  EXPECT_LT(kindle[1785].low   - 25420, epsilon);
  EXPECT_LT(kindle[1785].close - 25435, epsilon);
  EXPECT_EQ(kindle[1785].vol, 4800);
  EXPECT_LT(kindle[1785].interest - 108826, epsilon);
}

TEST(tick_file, read) {
  size_t size = 0;
  EXPECT_EQ(get_kindle_size("ru1701", KINDLE_PERIOD_TYPE_m1,
                  "/work/sean/projects/md/utest2", &size), 0);
  EXPECT_EQ(size, 1786);
  KINDLE* kindle = new KINDLE[size];
  EXPECT_EQ(0, read_kindle_file("ru1701", KINDLE_PERIOD_TYPE_m1, "/work/sean/projects/md/utest2", kindle, &size));
  EXPECT_EQ(size, 1786);

  verify_kindle_ru888(kindle);

  SafeDeleteAry(kindle);
}


TEST(tick_file, read_write) {
  size_t size = 0;
  EXPECT_EQ(get_kindle_size("ru1701", KINDLE_PERIOD_TYPE_m1,
                  "/work/sean/projects/md/utest2", &size), 0);
  EXPECT_EQ(size, 1786);
  KINDLE* kindle = new KINDLE[size];
  EXPECT_EQ(0, read_kindle_file("ru1701", KINDLE_PERIOD_TYPE_m1, "/work/sean/projects/md/utest2", kindle, &size));
  EXPECT_EQ(size, 1786);

  verify_kindle_ru888(kindle);

  EXPECT_EQ(save_kindle_file("ru1701", KINDLE_PERIOD_TYPE_m1, "/work/sean/projects/build/md", kindle, size), 0);

  char diff[] = {"diff /work/sean/projects/build/md/ru888.m1 /work/sean/projects/md/utest2/ru888.m1"};
  FILE* pp = popen(diff, "r");
  EXPECT_NE(pp, (FILE*)0);

  char buf[100];
  memset(buf, 0, 100);
  fread(buf, sizeof(char), 100, pp);
  EXPECT_EQ(((strcmp("", buf) == 0) || (strcmp("\n", buf) == 0)), true);
  EXPECT_NE(-1, pclose(pp));

  EXPECT_EQ(remove("/work/sean/projects/build/md/ru888.m1"), 0);

  SafeDeleteAry(kindle);
}


TEST(kindle_file, convert) {
  size_t size = 0;
  EXPECT_EQ(get_kindle_size("ru1701", KINDLE_PERIOD_TYPE_m1,
                  "/work/sean/projects/md/utest2", &size), 0);
  EXPECT_EQ(size, 1786);
  KINDLE* kindle = new KINDLE[size];
  EXPECT_EQ(0, read_kindle_file("ru1701", KINDLE_PERIOD_TYPE_m1, "/work/sean/projects/md/utest2", kindle, &size));
  EXPECT_EQ(size, 1786);

  verify_kindle_ru888(kindle);
  convert_kindle("ru1701", kindle, size, KINDLE_PERIOD_TYPE_m5 , "/work/sean/projects/md/utest2", nullptr);
  convert_kindle("ru1701", kindle, size, KINDLE_PERIOD_TYPE_m10, "/work/sean/projects/md/utest2", nullptr);
  convert_kindle("ru1701", kindle, size, KINDLE_PERIOD_TYPE_m30, "/work/sean/projects/md/utest2", nullptr);
  convert_kindle("ru1701", kindle, size, KINDLE_PERIOD_TYPE_h1 , "/work/sean/projects/md/utest2", nullptr);
  convert_kindle("ru1701", kindle, size, KINDLE_PERIOD_TYPE_h4 , "/work/sean/projects/md/utest2", nullptr);
  convert_kindle("ru1701", kindle, size, KINDLE_PERIOD_TYPE_D1 , "/work/sean/projects/md/utest2", nullptr);
  convert_kindle("ru1701", kindle, size, KINDLE_PERIOD_TYPE_W1 , "/work/sean/projects/md/utest2", nullptr);
  convert_kindle("ru1701", kindle, size, KINDLE_PERIOD_TYPE_M1 , "/work/sean/projects/md/utest2", nullptr);

  SafeDeleteAry(kindle);
}


TEST(convert, csv_to_m1_) {
  size_t amount = 0;
  EXPECT_EQ(0, tb_2_min("ag888", "/home/sean/Downloads/data-1m/ag888_1m.csv", "/work/sean/projects/build/md/data/cal/ag888.m1", &amount));
  EXPECT_EQ(0, tb_2_min("au888", "/home/sean/Downloads/data-1m/au888_1m.csv", "/work/sean/projects/build/md/data/cal/au888.m1", &amount));
  EXPECT_EQ(0, tb_2_min("CF888", "/home/sean/Downloads/data-1m/CF888_1m.csv", "/work/sean/projects/build/md/data/cal/CF888.m1", &amount));
  EXPECT_EQ(0, tb_2_min("i9888", "/home/sean/Downloads/data-1m/i9888_1m.csv", "/work/sean/projects/build/md/data/cal/i9888.m1", &amount));
  EXPECT_EQ(0, tb_2_min("IF888", "/home/sean/Downloads/data-1m/IF888_1m.csv", "/work/sean/projects/build/md/data/cal/IF888.m1", &amount));
  EXPECT_EQ(0, tb_2_min("j9888", "/home/sean/Downloads/data-1m/j9888_1m.csv", "/work/sean/projects/build/md/data/cal/j9888.m1", &amount));
  EXPECT_EQ(0, tb_2_min("m9888", "/home/sean/Downloads/data-1m/m9888_1m.csv", "/work/sean/projects/build/md/data/cal/m9888.m1", &amount));
  EXPECT_EQ(0, tb_2_min("ni888", "/home/sean/Downloads/data-1m/ni888_1m.csv", "/work/sean/projects/build/md/data/cal/ni888.m1", &amount));
  EXPECT_EQ(0, tb_2_min("p9888", "/home/sean/Downloads/data-1m/p9888_1m.csv", "/work/sean/projects/build/md/data/cal/p9888.m1", &amount));
  EXPECT_EQ(0, tb_2_min("rb888", "/home/sean/Downloads/data-1m/rb888_1m.csv", "/work/sean/projects/build/md/data/cal/rb888.m1", &amount));
  EXPECT_EQ(0, tb_2_min("ru888", "/home/sean/Downloads/data-1m/ru888_1m.csv", "/work/sean/projects/build/md/data/cal/ru888.m1", &amount));
  EXPECT_EQ(0, tb_2_min("SR888", "/home/sean/Downloads/data-1m/SR888_1m.csv", "/work/sean/projects/build/md/data/cal/SR888.m1", &amount));
}


TEST(convert, m1_to_all_kindle) {
  convert_kindle_data("ag888", "/work/sean/projects/build/md/data/cal");
  convert_kindle_data("au888", "/work/sean/projects/build/md/data/cal");
  convert_kindle_data("CF888", "/work/sean/projects/build/md/data/cal");
  convert_kindle_data("i9888", "/work/sean/projects/build/md/data/cal");
  convert_kindle_data("IF888", "/work/sean/projects/build/md/data/cal");
  convert_kindle_data("j9888", "/work/sean/projects/build/md/data/cal");
  convert_kindle_data("m9888", "/work/sean/projects/build/md/data/cal");
  convert_kindle_data("ni888", "/work/sean/projects/build/md/data/cal");
  convert_kindle_data("p9888", "/work/sean/projects/build/md/data/cal");
  convert_kindle_data("rb888", "/work/sean/projects/build/md/data/cal");
  convert_kindle_data("ru888", "/work/sean/projects/build/md/data/cal");
  convert_kindle_data("SR888", "/work/sean/projects/build/md/data/cal");
}


TEST(convert, ni888_m1_to_other) {
    const char* code = {"ni888"};
    const char* kindle_path = {"/work/sean/projects/build/md/data/cal"};

    size_t size = 0;
    EXPECT_EQ(get_kindle_size(code, KINDLE_PERIOD_TYPE_m1,
                   kindle_path, &size), 0);
    KINDLE* kindle = new KINDLE[size];
    EXPECT_EQ(0, read_kindle_file(code, KINDLE_PERIOD_TYPE_m1, kindle_path, kindle, &size));
    EXPECT_EQ(size, 172019);


    //for (int i = 5; i <= 13; ++i) //from KINDLE_PERIOD_TYPE_m5 to KINDLE_PERIOD_TYPE_1M
    int i = get_period_index(KINDLE_PERIOD_TYPE_M1);
      convert_kindle(code, kindle, size, gktype[i], "/work/sean/projects/build/md/data/cal", "/work/sean/projects/build/md/data/cal");

    SafeDeleteAry(kindle);
}
*********************************************************************************************************/


/********************************************************************************************************
#include <Python.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

int multiply(const char* module_name, const char* fun_name, const int p1, const int p2)
{
    printf("init python ... ");
    Py_Initialize();
    printf("done\n");

    printf("Hello World! %s, %s, %d, %d\n", module_name, fun_name, p1, p2);
    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;

    printf("py-string from string ... ");
    pName = PyString_FromString(module_name);
    printf("done %lx, %lu\n", (int64_t)(pName), sizeof(pName));

    printf("py-import ... ");
    pModule = PyImport_Import(pName);
    printf("done\n");
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, fun_name);

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(2);
            pValue = PyInt_FromLong(p1);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            PyTuple_SetItem(pArgs, 0, pValue);

            pValue = PyInt_FromLong(p2);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 2;
            }
            PyTuple_SetItem(pArgs, 1, pValue);

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", fun_name);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", module_name);
        return 1;
    }
    Py_Finalize();
    return 0;
}


extern int run_multiply(const char* module_name, const char* fun_name, const int p1, const int p2);

#ifdef __cplusplus
}
#endif //__cplusplus


TEST(C_PYTHON, c_all_multiply) {
  printf ("Directly Call multiply\n");
  EXPECT_EQ(0, multiply("py1", "multiply", 2, 3));
  //printf ("\nCall multiply in shared libray\n");
  //EXPECT_EQ(0, run_multiply("py1", "multiply", 2, 3));
  printf ("\n");
}
TEST(KindleFile, read_file) {
    size_t size = 0;
    EXPECT_EQ(get_kindle_size("rb1701", CANDLE_PERIOD_TYPE_m1,
                    "/work/sean/projects/build/md/data/cal", &size), 0);

    KINDLE* kindle = new KINDLE[size];
    EXPECT_EQ(0, read_kindle_file("rb1701", CANDLE_PERIOD_TYPE_m1, "/work/sean/projects/build/md/data/cal", kindle, &size));

    SafeDeleteAry(kindle);
}
**********************************************************************************************************/

#include "dataset.h"


TEST(DataFrame, construct) {
  const size_t max_size = 100;
  CandleInfo cdi;
  memset(&cdi, 0, sizeof(Candle));

  size_t memory_size = TSDATAFRAME(Candle)::get_memory_size(max_size);
  EXPECT_EQ(memory_size, sizeof(Candle) * max_size * 2 + sizeof(CandleInfo));
  char* memptr = new char[memory_size];

  cdi.di.cbsize = sizeof(Candle);
  cdi.di.pos = 100 - 1;
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
    cdi.di.pos = 100 - 1;
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

int main(int argc, char **argv) {

#ifdef _WINDOWS
#ifdef _DEBUG_
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  _CrtDumpMemoryLeaks();
#endif //_DEBUG_
#endif //_WINDOWS

//  if (argc != 2) {
//    printf("Usage testing <json config file>\n");
//    return (0);
//  }

  //strcpy_s(confpath, MAX_PATH, argv[1]);
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

/*
diff SR888.m5.csv  bak/SR888.m5.csv
diff SR888.m10.csv bak/SR888.m10.csv
diff SR888.m15.csv bak/SR888.m15.csv
diff SR888.m30.csv bak/SR888.m30.csv
diff SR888.h1.csv  bak/SR888.h1.csv
diff SR888.h4.csv  bak/SR888.h4.csv
diff SR888.D1.csv  bak/SR888.D1.csv
diff SR888.W1.csv  bak/SR888.W1.csv
diff SR888.M1.csv  bak/SR888.M1.csv

diff ni888.m5.csv  bak/ni888.m5.csv
diff ni888.m10.csv bak/ni888.m10.csv
diff ni888.m15.csv bak/ni888.m15.csv
diff ni888.m30.csv bak/ni888.m30.csv
diff ni888.h1.csv  bak/ni888.h1.csv
diff ni888.h4.csv  bak/ni888.h4.csv
diff ni888.D1.csv  bak/ni888.D1.csv
diff ni888.W1.csv  bak/ni888.W1.csv
diff ni888.M1.csv  bak/ni888.M1.csv


diff IF888.m5.csv  bak/IF888.m5.csv
diff IF888.m10.csv bak/IF888.m10.csv
diff IF888.m15.csv bak/IF888.m15.csv
diff IF888.m30.csv bak/IF888.m30.csv
diff IF888.h1.csv  bak/IF888.h1.csv
diff IF888.h4.csv  bak/IF888.h4.csv
diff IF888.D1.csv  bak/IF888.D1.csv
diff IF888.W1.csv  bak/IF888.W1.csv
diff IF888.M1.csv  bak/IF888.M1.csv


diff rb888.m5.csv  bak/rb888.m5.csv
diff rb888.m10.csv bak/rb888.m10.csv
diff rb888.m15.csv bak/rb888.m15.csv
diff rb888.m30.csv bak/rb888.m30.csv
diff rb888.h1.csv  bak/rb888.h1.csv
diff rb888.h4.csv  bak/rb888.h4.csv
diff rb888.D1.csv  bak/rb888.D1.csv
diff rb888.W1.csv  bak/rb888.W1.csv
diff rb888.M1.csv  bak/rb888.M1.csv
*/

