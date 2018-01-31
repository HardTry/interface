#include "utility.h"

#include <iostream>
#include <cmath>
#include <cfloat>
#include <fstream>
#include <iostream>
#include <string>

#include "rapidjson/document.h"

using namespace std;
/**
 * @brief TimeTrans::make_the_time
 * @param TradeDate in yyyymmmdd(20100906) fromat
 * @param TradeTime in hh:mm:ss(17:17:27) format if format_type is 1
 *        in hhmmss format if format_type is 2
 * @param format_type
 * @return the time in time_t format
 */
time_t make_the_time(char *TradeDate, char *TradeTime) { //, int format_type) {
  int llt = atoi(TradeDate);
  struct tm lt;
  memset(&lt, 0, sizeof(lt));

  lt.tm_year = (int)(llt / 10000) - 1900;
  lt.tm_mon = (int)(llt - (lt.tm_year + 1900) * 10000) / 100 - 1;
  lt.tm_mday = (int)(llt - (lt.tm_year + 1900) * 10000 - (lt.tm_mon + 1) * 100);

  // if (format_type == 1) {
  lt.tm_hour = (TradeTime[0] - '0') * 10 + (TradeTime[1] - '0');
  lt.tm_min = (TradeTime[3] - '0') * 10 + (TradeTime[4] - '0');
  lt.tm_sec = (TradeTime[6] - '0') * 10 + (TradeTime[7] - '0');
  //} else {
  //  // format_type == 2
  //  lt.tm_hour = (TradeTime[0] - '0') * 10 + (TradeTime[1] - '0');
  //  lt.tm_min =  (TradeTime[2] - '0') * 10 + (TradeTime[3] - '0');
  //  lt.tm_sec =  (TradeTime[4] - '0') * 10 + (TradeTime[5] - '0');
  //}

  return mktime(&lt);
}

time_t make_the_time(char *TradeDate, uint32_t TradeTime) {
  uint32_t llt = atoi(TradeDate);
  struct tm lt;
  memset(&lt, 0, sizeof(lt));

  lt.tm_year = (int)(llt / 10000) - 1900;
  lt.tm_mon = (int)(llt - (lt.tm_year + 1900) * 10000) / 100 - 1;
  lt.tm_mday = (int)(llt - (lt.tm_year + 1900) * 10000 - (lt.tm_mon + 1) * 100);

  lt.tm_hour = (int)(TradeTime / 10000);
  lt.tm_min = (int)((TradeTime - lt.tm_hour * 10000) / 100);
  lt.tm_sec = (int)(TradeTime - (lt.tm_hour * 10000) - lt.tm_min * 100);
  return mktime(&lt);
}

time_t make_the_time(uint32_t TradeDate, uint32_t TradeTime) {
  uint32_t llt = TradeDate;
  struct tm lt;

  lt.tm_year = (int)(llt / 10000) - 1900;
  lt.tm_mon = (int)(llt - (lt.tm_year + 1900) * 10000) / 100 - 1;
  lt.tm_mday = (int)(llt - (lt.tm_year + 1900) * 10000 - (lt.tm_mon + 1) * 100);

  lt.tm_hour = (int)(TradeTime / 10000);
  lt.tm_min = (int)((TradeTime - lt.tm_hour * 10000) / 100);
  lt.tm_sec = (int)(TradeTime - (lt.tm_hour * 10000) - lt.tm_min * 100);
  return mktime(&lt);
}

time_t make_the_time(time_t tt, uint32_t TradeTime) {
  struct tm lt;
  localtime_s(&lt, &tt);

  lt.tm_hour = (int)(TradeTime / 10000);
  lt.tm_min = (int)((TradeTime - lt.tm_hour * 10000) / 100);
  lt.tm_sec = (int)(TradeTime - (lt.tm_hour * 10000) - lt.tm_min * 100);
  return mktime(&lt);
}


void outbuf(DATAPTR data, size_t length) {
  unsigned char *ptr = reinterpret_cast<unsigned char *>(data);
  for (size_t i = 0; i < length; ++i) {
    printf("%02x ", ptr[i]); // & 0x000000FF);
    if (!((i + 1) % 16))
      printf("\n");
  }
  printf("\n");
}

//
// bool cmpstr(multimap<string, string>::iterator *map1,
//            multimap<string, string>::iterator *map2) {
//  return (*map1)->second < (*map2)->second;
//}
//
std::string minString(std::multimap<std::string, std::string> &mapInst,
                      std::string codeInst) {
  // pairMap is pair<multimap<string, string>::iterator,
  //                 multimap<string, string>::iterator>
  auto pairMap = mapInst.equal_range(codeInst);

  std::vector<string> vecInstrument;
  for (multimap<string, string>::iterator it2 = pairMap.first;
       it2 != pairMap.second; ++it2)
    vecInstrument.push_back((*it2).second);
  if (vecInstrument.size() > 0)
    return *std::min_element(vecInstrument.begin(), vecInstrument.end());
  else
    return "";

  // return *std::min_element(pairMap.first, pairMap.second, cmpstr);
}

int match_index(std::vector<regex> &regexInstruments, const char *str) {
  for (int i = 0; i < (int)regexInstruments.size(); ++i)
    if (regex_match(str, regexInstruments[i]))
      return i;

  return (-1);
}

/**
 * @brief getInstCode return the code of a instrument
 * @param InstRegex the regex of a instrument
 * @return the code
 */
std::string getInstCode(std::string instrument) {
  std::string code("");
  for (auto c : instrument) {
    if (!isalpha(c))
      break;
    code.push_back(c);
  }
  return code;
}

/**
 * @brief getInstContinuedCode return the continued  code of a instrument
 * @param InstRegex the regex of a instrument
 * @return the continued  code
 */
const std::string getInstContinuedCode(std::string instrument) {
  std::string code("");
  for (auto c : instrument) {
    if (!isalpha(c))
      break;
    code.push_back(c);
  }
  code += (code.length() < 2) ? "9888" : "888";

  return code;
}

time_t get_exit_time(const char *the_time) {
  struct tm lt;
  time_t tt = time(0);
  localtime_s(&lt, &tt);

  char *strtt = strdup(the_time);
  char *s = strchr(strtt, ':');
  *s = 0;
  lt.tm_hour = atoi(strtt);
  *s = ':';
  char *t = s + 1;
  s = strchr(t, ':');
  *s = 0;
  lt.tm_min = atoi(t);
  *s = ':';
  t = s + 1;
  lt.tm_sec = atoi(t);
  free(strtt);
  return mktime(&lt);
}

int tb_2_min(const char *instrument, const char *tbcsv, const char *minfile,
             size_t *size) {
  FILE *fpIn = 0, *fpOut = 0;
  if (fopen_s(&fpIn, tbcsv, "r"))
    return -1;

  size_t amount = 0;
  CANDLE_PERIOD_TYPE period = CANDLE_PERIOD_TYPE_m1;

  char buf[1024];
  while (fgets(buf, 1024, fpIn)) {
    if ((*buf == 0) || (*buf == '#') || (*buf == '\r') || (*buf == '\n'))
      continue;
    amount++;
  }
  fseek(fpIn, 0, SEEK_SET);

  if (fopen_s(&fpOut, minfile, "wb"))
    return -2;
  size_t n = fwrite(instrument, sizeof(char), strlen(instrument) + 1, fpOut);
  if (n != strlen(instrument) + 1)
    return -10;
  n = fwrite(&period, sizeof(CANDLE_PERIOD_TYPE), 1, fpOut);
  if (n != 1)
    return -11;
  n = fwrite(&amount, sizeof(size_t), 1, fpOut);
  if (n != 1)
    return -12;

  KINDLE kindle;
  memset(&kindle, 0, sizeof(kindle));

  char date[32];
  double clock;

  int ret = 0;
  size_t real_amount = 0;
  size_t lineno = 0;
  while (!ret && fgets(buf, 1024, fpIn)) {
    ++lineno;
    if ((*buf == 0) || (*buf == '#') || (*buf == '\r') || (*buf == '\n'))
      continue;

    int n = sscanf(buf, "%[^,],%lf,%lf,%lf,%lf,%lf,%u,%lf", date, &clock,
                   &kindle.open, &kindle.high, &kindle.low, &kindle.close,
                   &kindle.volume, &kindle.interest);

    if (n != 8) {
      ret = -4;
      break;
    }

    kindle.UpdateTime =
        make_the_time(date, static_cast<uint32_t>(clock * 1000000));

    if (1 != fwrite(&kindle, sizeof(kindle), 1, fpOut)) {
      printf("write kindle error\n");
      ret = -3;
      break;
    }
    ++real_amount;
  }

  fclose(fpOut);
  fclose(fpIn);

  if (!ret && real_amount != amount)
    ret = -100;
  if (!ret)
    *size = real_amount;
  return ret;
}

static void init_kindle(KINDLE *kOut, KINDLE *kIn, struct tm *old_tm,
                        struct tm *new_tm, CANDLE_PERIOD_TYPE period) {
  memset(kOut, 0, sizeof(KINDLE));
  memcpy(kOut, kIn, sizeof(KINDLE));
  kOut->low = 1e10;

  if (period >= CANDLE_PERIOD_TYPE_m5 && period <= CANDLE_PERIOD_TYPE_h4) {
    kOut->UpdateTime = kIn->UpdateTime - kIn->UpdateTime % period;

    if (period == CANDLE_PERIOD_TYPE_h4)
      fix_open_time_to_trading_time(kOut, new_tm);
  } else
    kOut->UpdateTime = kIn->UpdateTime;

  localtime_s(old_tm, &kOut->UpdateTime);
}

void fix_open_time_to_trading_time(KINDLE *k, struct tm *ttm) {
  struct tm zero;
  memcpy(&zero, ttm, sizeof(struct tm));
  zero.tm_hour = 0, zero.tm_min = 0, zero.tm_sec = 0;
  time_t t_0000 = mktime(&zero);
  zero.tm_hour = 1;
  time_t t_0100 = mktime(&zero);
  zero.tm_hour = 9;
  time_t t_0900 = mktime(&zero);
  zero.tm_hour = 11, zero.tm_min = 30;
  time_t t_1130 = mktime(&zero);
  zero.tm_hour = 13, zero.tm_min = 0;
  time_t t_1300 = mktime(&zero);
  zero.tm_hour = 15;
  time_t t_1500 = mktime(&zero);
  zero.tm_hour = 16;
  time_t t_1600 = mktime(&zero);
  zero.tm_hour = 21;
  time_t t_2100 = mktime(&zero);

  // for h1, h4 .......
  if (k->UpdateTime >= t_0000 && k->UpdateTime <= t_0100)
    k->UpdateTime = t_0000 - 3 * 60 * 60; // previous day 21:00
  else {
    if (k->UpdateTime >= t_1600)
      k->UpdateTime = t_2100;
    else if (k->UpdateTime >= t_1500)
      k->UpdateTime = t_1500;
    else if (k->UpdateTime > t_1130 && k->UpdateTime <= t_1300)
      k->UpdateTime = t_1300;
    else if (k->UpdateTime > t_0100 && k->UpdateTime <= t_0900)
      k->UpdateTime = t_0900;
  }
}

#define is_next_day(n, o)                                                      \
  ((n.tm_yday > o.tm_yday) || (n.tm_mon > o.tm_mon) || (n.tm_year > o.tm_year))

int get_days_in_month(const struct tm *ttm) {
  switch (ttm->tm_mon + 1) {
  case 1:
  case 3:
  case 5:
  case 7:
  case 8:
  case 10:
  case 12:
    return 31;
  case 4:
  case 6:
  case 9:
  case 11:
    return 30;
  case 2: {
    int Y = ttm->tm_year + 1900;
    if (!(Y % 100)) {
      return (!(Y % 400)) ? 29 : 28;
    } else {
      return (!(Y % 4)) ? 29 : 28;
    }
  }
  }
  return 0;
}

bool over_one_month(const struct tm *ntm, time_t n, const struct tm *otm,
                    time_t o, time_t prev) {
  if ((ntm->tm_mon - otm->tm_mon >= 2) ||
      (ntm->tm_year > otm->tm_year && (12 - otm->tm_mon + ntm->tm_mon) >= 2))
    return true;

  struct tm ttm;
  memcpy(&ttm, ntm, sizeof(struct tm));
  ttm.tm_hour = ttm.tm_min = ttm.tm_sec = 0;
  time_t endt = mktime(&ttm) +
                (get_days_in_month(ntm) - (ntm->tm_mday - 1)) * 24 * 60 * 60 -
                1; // last second in the month
  localtime_s(&ttm, &endt);
  if (ttm.tm_wday == 0)
    endt -= (2 * 24 + 3) * 60 * 60;
  else if (ttm.tm_wday == 6)
    endt -= (24 + 3) * 60 * 60;
  else
    endt -= 3 * 60 * 60;
  // localtime_s(&ttm, &endt);

  if (n >= endt && n - o > 24 * 60 * 60)
    return true;
  else {
    time_t ppp = prev;
    localtime_s(&ttm, &ppp);
    if ((ntm->tm_mon > ttm.tm_mon || ntm->tm_year > ttm.tm_year) &&
        (ttm.tm_hour <= 15) && (ntm->tm_hour >= 9))
      return true;
  }
  return false;
}

bool over_period(time_t now, time_t start, struct tm *new_tm, struct tm *old_tm,
                 CANDLE_PERIOD_TYPE period) {
  if (((now - start) >= period) || ((new_tm->tm_yday > old_tm->tm_yday) ||
                                    (new_tm->tm_mon > old_tm->tm_mon) ||
                                    (new_tm->tm_year > old_tm->tm_year))) {
    if (period != CANDLE_PERIOD_TYPE_h4)
      return true;
    else {
      if ((now - start) >= CANDLE_PERIOD_TYPE_h4)
        return true;
    }
  }
  return false;
}

bool over_one_week(struct tm *new_tm, struct tm *old_tm, time_t now,
                   time_t start) {
  struct tm tm0;
  time_t zero = 0;
  memcpy(&tm0, old_tm, sizeof(struct tm));
  time_t nextday = now + 3 * 3600 + 1;
  struct tm tmnd;
  localtime_s(&tmnd, &nextday);
  if (tmnd.tm_wday == 6 && now - start > 4 * 60 * 60)
    return true;
  else {
    tm0.tm_hour = 0, tm0.tm_min = 0, tm0.tm_sec = 0;
    zero = mktime(&tm0);
    zero -= tm0.tm_wday * 24 * 60 * 60;
    if (now - zero >= 6 * 24 * 60 * 60 && new_tm->tm_hour >= 9 &&
        (now - start) / (24 * 60 * 60) > 6)
      return true;
  }
  return false;
}

bool over_one_day(struct tm *old_tm, time_t now) {
  struct tm tm0;
  time_t zero = 0;
  memcpy(&tm0, old_tm, sizeof(struct tm));

  if (old_tm->tm_hour >= 21 && old_tm->tm_hour <= 23) {
    tm0.tm_hour = 21, tm0.tm_min = 0, tm0.tm_sec = 0;
    zero = mktime(&tm0);
  } else if (old_tm->tm_hour >= 9 && old_tm->tm_hour <= 15) {
    tm0.tm_hour = 0, tm0.tm_min = 0, tm0.tm_sec = 0;
    zero =
        mktime(&tm0) - 3 * 60 * 60; // before 3 hours, is privous day's 21:00:00
  }
  return (now - zero >= 24 * 60 * 60);
}

/**
 * @brief kindle_min_conveter convert the kindle data from 1 minute period to
 * other period
 * @param inM1 the input kindle data
 * @param inLen the length of kindle data
 * @param out the output kindle data
 * @param outLen the length of kindle data
 * @param period the new period
 * @return zero for good
 */
int kindle_min_conveter(KINDLE *inM1, size_t inLen, KINDLE *out, size_t *outLen,
                        CANDLE_PERIOD_TYPE period) {
  if (period < CANDLE_PERIOD_TYPE_m1)
    return -1;
  if (period > CANDLE_PERIOD_TYPE_M1)
    return -2;
  if (!inM1 || !out || !inLen || !(*outLen))
    return -3;

  size_t j = 0;
  struct tm old_tm, new_tm;
  init_kindle(out, inM1, &old_tm, &new_tm, period);
  bool kindle_closed = false;

  for (size_t i = 1; i < inLen && j < *outLen; ++i) {
#ifdef _DEBUG_
// if (i >= (size_t)39007)
//   int kkkkkkk = 0;
#endif //_DEBUG_
    localtime_s(&new_tm, &inM1[i].UpdateTime);

    kindle_closed = false;

    if (period >= CANDLE_PERIOD_TYPE_m5 && period <= CANDLE_PERIOD_TYPE_h4) {
      kindle_closed = over_period(inM1[i].UpdateTime, out[j].UpdateTime,
                                  &new_tm, &old_tm, period);
    } else {
      if (period == CANDLE_PERIOD_TYPE_W1) {
        kindle_closed = over_one_week(&new_tm, &old_tm, inM1[i].UpdateTime,
                                      out[j].UpdateTime);
      } else if (period == CANDLE_PERIOD_TYPE_D1) {
        kindle_closed = over_one_day(&old_tm, inM1[i].UpdateTime);
      } else if (period == CANDLE_PERIOD_TYPE_M1) {
        kindle_closed =
            over_one_month(&new_tm, inM1[i].UpdateTime, &old_tm,
                           out[j].UpdateTime, inM1[i - 1].UpdateTime);
      }
    }

    if (kindle_closed) {
      init_kindle(out + (++j), inM1 + i, &old_tm, &new_tm, period);
    } else {
#ifdef _UNIX
      out[j].low = std::min(out[j].low, inM1[i].low);
      out[j].high = std::max(out[j].high, inM1[i].high);
#else
      out[j].low = min(out[j].low, inM1[i].low);
      out[j].high = max(out[j].high, inM1[i].high);
#endif //_WINDOWS
      out[j].interest = inM1[i].interest;
      out[j].volume += inM1[i].volume;
      out[j].close = inM1[i].close;
    }
  }

  if (!kindle_closed)
    ++j;
  *outLen = j;

  return (0);
}

const char *kperiod_postfix[] = {
    "",     ".s10", ".s15", ".s30", ".m1", ".m5", ".m10", ".m15",
    ".m30", ".h1",  ".h4",  ".D1",  ".W1", ".M1", ".Y1",
};

const CANDLE_PERIOD_TYPE gkperiod[] = {
    CANDLE_PERIOD_TYPE_UNKNOW, CANDLE_PERIOD_TYPE_s10, CANDLE_PERIOD_TYPE_s15,
    CANDLE_PERIOD_TYPE_s30,    CANDLE_PERIOD_TYPE_m1,  CANDLE_PERIOD_TYPE_m5,
    CANDLE_PERIOD_TYPE_m10,    CANDLE_PERIOD_TYPE_m15, CANDLE_PERIOD_TYPE_m30,
    CANDLE_PERIOD_TYPE_h1,     CANDLE_PERIOD_TYPE_h4,  CANDLE_PERIOD_TYPE_D1,
    CANDLE_PERIOD_TYPE_W1,     CANDLE_PERIOD_TYPE_M1,  CANDLE_PERIOD_TYPE_Y1,
};

int get_period_index(CANDLE_PERIOD_TYPE period) {
  switch (period) {
  case CANDLE_PERIOD_TYPE_UNKNOW:
    return (0);
  case CANDLE_PERIOD_TYPE_s10:
    return (1);
  case CANDLE_PERIOD_TYPE_s15:
    return (2);
  case CANDLE_PERIOD_TYPE_s30:
    return (3);
  case CANDLE_PERIOD_TYPE_m1:
    return (4);
  case CANDLE_PERIOD_TYPE_m5:
    return (5);
  case CANDLE_PERIOD_TYPE_m10:
    return (6);
  case CANDLE_PERIOD_TYPE_m15:
    return (7);
  case CANDLE_PERIOD_TYPE_m30:
    return (8);
  case CANDLE_PERIOD_TYPE_h1:
    return (9);
  case CANDLE_PERIOD_TYPE_h4:
    return (10);
  case CANDLE_PERIOD_TYPE_D1:
    return (11);
  case CANDLE_PERIOD_TYPE_W1:
    return (12);
  case CANDLE_PERIOD_TYPE_M1:
    return (13);
  case CANDLE_PERIOD_TYPE_Y1:
    return (14);
  }
  return 0;
}

int save_kindle_file(const char *instrument, CANDLE_PERIOD_TYPE period,
                     const char *store_path, KINDLE *kindle, size_t size) {
  FILE *fp = 0;
  char filepath[MAX_PATH];
  string continued_code = getInstContinuedCode(instrument);
  snprintf(filepath, MAX_PATH, "%s/%s%s", store_path, continued_code.c_str(),
           kperiod_postfix[get_period_index(period)]);
  if (fopen_s(&fp, filepath, "w+b")) {
    fprintf(stderr, "can not open %s for write, %s, %s\n", filepath, store_path,
            continued_code.c_str());
    return -1;
  }

  size_t n = fwrite(continued_code.c_str(), sizeof(char),
                    continued_code.length() + 1, fp);
  if (n != continued_code.length() + 1)
    return -10;
  n = fwrite(&period, sizeof(CANDLE_PERIOD_TYPE), 1, fp);
  if (n != 1)
    return -11;
  n = fwrite(&size, sizeof(size_t), 1, fp);
  if (n != 1)
    return -12;

  bool ret = (fwrite(kindle, sizeof(KINDLE), size, fp) == size);
  fclose(fp);
  return (ret) ? (0) : -1;
}

int read_kindle_file(const char *instrument, CANDLE_PERIOD_TYPE period,
                     const char *store_path, KINDLE *kindle, size_t *size) {
  FILE *fp = 0;
  char filepath[MAX_PATH];

  char buf[1024];
  memset(buf, 0, 1024);
  size_t len = 20 + sizeof(CANDLE_PERIOD_TYPE) + sizeof(size_t);

  std::string instcode = getInstContinuedCode(instrument);
  snprintf(filepath, MAX_PATH, "%s/%s%s", store_path, instcode.c_str(),
           kperiod_postfix[get_period_index(period)]);

  int ret = 0;
  if (!ret && fopen_s(&fp, filepath, "rb"))
    ret = -1;
  if (!ret && (len != fread(buf, sizeof(char), len, fp)))
    ret = -2;
  if (!ret && strcmp(buf, instcode.c_str()))
    ret = -3;

  size_t amount = 0;
  len = strlen(buf) + 1;
  if (!ret) {
    CANDLE_PERIOD_TYPE my_period =
        *(reinterpret_cast<CANDLE_PERIOD_TYPE *>(buf + len));
    amount =
        *(reinterpret_cast<size_t *>(buf + len + sizeof(CANDLE_PERIOD_TYPE)));

    if (!ret && (period != my_period))
      ret = -4;
    if (!ret && (*size > 0) && (amount > *size))
      ret = -5;
  }

  size_t real_size = 0;
  if (!ret) {
    fseek(fp, (long)(len + sizeof(CANDLE_PERIOD_TYPE) + sizeof(size_t)), SEEK_SET);
    real_size = fread(kindle, sizeof(KINDLE), amount, fp);
    if (amount != real_size) {
      printf("read kindle error 6, %s %d\n", instrument, period);
      ret = -6;
    }
  }

  *size = real_size;

  fclose(fp);
  return ret;
}

int get_kindle_size(const char *instrument, CANDLE_PERIOD_TYPE period,
                    const char *store_path, size_t *size) {
  FILE *fp = 0;
  char filepath[MAX_PATH];

  char buf[1024];
  memset(buf, 0, 1024);
  size_t len = 20 + sizeof(CANDLE_PERIOD_TYPE) + sizeof(size_t);

  std::string instcode = getInstContinuedCode(instrument);
  snprintf(filepath, MAX_PATH, "%s/%s%s", store_path, instcode.c_str(),
           kperiod_postfix[get_period_index(period)]);

  int ret = 0;
  if (!ret && fopen_s(&fp, filepath, "rb")) {
    printf("Can not Open %s\n", filepath);
    ret = -1;
  }
  if (!ret && (len != fread(buf, sizeof(char), len, fp)))
    ret = -2;
  if (!ret && strcmp(buf, instcode.c_str()))
    ret = -3;

  if (!ret) {
    len = strlen(buf) + 1;
    *size =
        *(reinterpret_cast<size_t *>(buf + len + sizeof(CANDLE_PERIOD_TYPE)));
  }

  if (fp)
    fclose(fp);
  return ret;
}

int save_kindle_csv(const char *instrument, CANDLE_PERIOD_TYPE period,
                    const char *store_path, KINDLE *kindle, size_t size) {
  FILE *fp = 0;
  char filepath[MAX_PATH];
  string continued_code = getInstContinuedCode(instrument);
  snprintf(filepath, MAX_PATH, "%s/%s%s.csv", store_path,
           continued_code.c_str(), kperiod_postfix[get_period_index(period)]);
  if (fopen_s(&fp, filepath, "w+"))
    return -1;

  int ret = 0;
  for (size_t i = 0; (ret >= 0) && i < size; ++i) {
    struct tm ttm;
    localtime_s(&ttm, &kindle[i].UpdateTime);
    ret = fprintf(
        fp, "%04d%02d%02d, 0.%02d%02d, %.02f, %.02f, %.02f, %.02f, %u, %.02f\n",
        ttm.tm_year + 1900, ttm.tm_mon + 1, ttm.tm_mday, ttm.tm_hour,
        ttm.tm_min, kindle[i].open, kindle[i].high, kindle[i].low,
        kindle[i].close, kindle[i].volume, kindle[i].interest);
  }

  fclose(fp);
  return (ret >= 0) ? (0) : -2;
}

int convert_kindle(const char *code, KINDLE *kindle, size_t size,
                   CANDLE_PERIOD_TYPE period, const char *csvpath,
                   const char *kindle_path) {
  // size_t p = period / 60;
  size_t outLen = size;
  KINDLE *out = new KINDLE[outLen];
  --outLen;
  if (0 != kindle_min_conveter(kindle, size, out, &outLen, period))
    return -1;
  if (csvpath != nullptr)
    if (0 != save_kindle_csv(code, period, csvpath, out, outLen))
      return -2;
  if (kindle_path != nullptr)
    if (0 != save_kindle_file(code, period, kindle_path, out, outLen))
      return -3;

  SafeDeleteAry(out);
  return 0;
}

int convert_kindle_data(const char *code, const char *kindle_path) {
  size_t size = 0;
  if (get_kindle_size(code, CANDLE_PERIOD_TYPE_m1, kindle_path, &size) != 0) {
    printf("Error get kindle size!\n");
    return -1;
  }

  KINDLE *kindle = new KINDLE[size];
  if (0 != read_kindle_file(code, CANDLE_PERIOD_TYPE_m1, kindle_path, kindle,
                            &size)) {
    printf("Error read kindle file!\n");
    return -2;
  }

  for (int i = 5; i <= 13;
       ++i) // from KINDLE_PERIOD_TYPE_m5 to KINDLE_PERIOD_TYPE_1M
    if (convert_kindle(code, kindle, size, gkperiod[i], nullptr, kindle_path) !=
        0)
      printf("period %d convert error\n", gkperiod[i]);

  SafeDeleteAry(kindle);

  return 0;
}


int compare_files(const char* file1, const char* file2) {
  FILE* fp1 = fopen(file1, "rb");
  if (!fp1)
    return -1001;
  FILE* fp2 = fopen(file2, "rb");
  if (!fp2) {
    fclose(fp1);
    return -1002;
  }

  fseek(fp1, 0, SEEK_END);
  long size1 = ftell(fp1);
  fseek(fp1, 0, SEEK_SET);

  fseek(fp2, 0, SEEK_END);
  long size2 = ftell(fp2);
  fseek(fp2, 0, SEEK_SET);

  int ret = 0;
  if (size1 != size2)
      ret = (size1 > size2) ? 5 : -5;

  if (!ret) {
    char c1, c2;
    for (long i = 0; !ret && i < size1; ++i) {
      if (1 != fread(&c1, 1, 1, fp1))
        ret = -1003;

      if (1 != fread(&c2, 1, 1, fp2))
        ret = -1004;

      if (!ret && (c1 != c2)) {
        ret = (c1 > c2) ? 1 : -1;
        printf("not same at %ld\n", i);
      }
    }
  }

  fclose(fp2);
  fclose(fp1);
  return ret;
}

std::vector<std::string> split(const string& input, const string& regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator
        first{input.begin(), input.end(), re, -1},
        last;
    return {first, last};
}
