#ifndef UTILITY_H
#define UTILITY_H

#include "utility_global.h"
#include "def.h"
#include "mderr.h"
#include "str.h"
#include "mylog.h"
#include "mem.h"
#include "dataset.h"


/**
 * @brief make_the_time converte 'yyyymmdd' and 'hh:mm:ss' to time_t
 * @param TradeDate the date
 * @param TradeTime the time
 * @return the time in time_t
 */
time_t UTILITY_API make_the_time(char *TradeDate, char *TradeTime);

/**
 * @brief make_the_time converte 'yyyymmdd' and 'hh:mm:ss' to time_t
 * @param TradeDate the date
 * @param TradeTime the time in 'hhmmss'
 * @return the time in time_t
 */
time_t UTILITY_API make_the_time(char *TradeDate, uint32_t TradeTime);

/**
 * @brief make_the_time converte 'yyyymmdd' and 'hh:mm:ss' to time_t
 * @param TradeDate the date
 * @param TradeTime the time in 'hhmmss'
 * @return the time in time_t
 */
time_t UTILITY_API make_the_time(uint32_t TradeDate, uint32_t TradeTime);

time_t UTILITY_API make_the_time(time_t tt, uint32_t TradeTime);

/**
 * @brief outbuf output the memory in hex
 * @param data, the pointer to memory
 * @param length the length of data
 */
void UTILITY_API outbuf(DATAPTR data, size_t length);

/**
 * @brief minString return minimal matched string in multimap
 * @param mapInst the multimap of regex to instrument
 * @param codeInst the code of instrument
 * @return the minimal code of instrument
 */
extern UTILITY_API std::string
minString(std::multimap<std::string, std::string> &mapInst,
          std::string codeInst);

/**
 * @brief match_index return str matched index in vector<regex>
 * @param regexInstruments the vector of regex
 * @param str the input string
 * @return the index in vector or -1 for did not match
 */
extern UTILITY_API int match_index(std::vector<regex> &regexInstruments,
                                   const char *str);

/**
 * @brief getInstCode return the code of a instrument
 * @param InstRegex the regex of a instrument
 * @return the code
 */
extern UTILITY_API std::string getInstCode(std::string instrument);

/**
 * @brief getInstContinuedCode return the continued code of a instrument
 * @param InstRegex the regex of a instrument
 * @return the continued  code
 */
const UTILITY_API std::string getInstContinuedCode(std::string instrument);

/**
 * @brief get_exit_time get time from time-string in "hh:mm:ss" format
 * @param the_time time-string
 * @return the exit time
 */
extern UTILITY_API time_t get_exit_time(const char *the_time);

/**
 * @brief tb_2_min convete TB-1min-data to Kindle Format
 * @param instrument the code of instrument
 * @param tbcsv the TB-1min-data file
 * @param minfile the Kindle file
 * @return zero for good
 */
extern UTILITY_API int tb_2_min(const char *instrument, const char *tbcsv,
                                const char *minfile, size_t *size);

/**
 * @brief kindle_min_conveter convert the kindle data from 1 minute period to
 * other period
 * @param inM1 the input kindle data
 * @param inLen the length of kindle data
 * @param out the output kindle data
 * @param outLen the length of kindle data
 * @param period the new period
 * @return zero for good and outLen is the length of out
 */
extern UTILITY_API int kindle_min_conveter(KINDLE *inM1, size_t inLen,
                                           KINDLE *out, size_t *outLen,
                                           CANDLE_PERIOD_TYPE period);

extern UTILITY_API int save_kindle_file(const char *instrument,
                                        CANDLE_PERIOD_TYPE period,
                                        const char *store_path, KINDLE *kindle,
                                        size_t size);

extern UTILITY_API int get_kindle_size(const char *instrument,
                                       CANDLE_PERIOD_TYPE period,
                                       const char *store_path, size_t *size);
extern UTILITY_API int read_kindle_file(const char *instrument,
                                        CANDLE_PERIOD_TYPE period,
                                        const char *store_path, KINDLE *kindle,
                                        size_t *size);

extern UTILITY_API const char *kperiod_postfix[];
extern UTILITY_API const CANDLE_PERIOD_TYPE gkperiod[];
extern UTILITY_API int get_period_index(CANDLE_PERIOD_TYPE period);
extern UTILITY_API int save_kindle_csv(const char *instrument,
                                       CANDLE_PERIOD_TYPE period,
                                       const char *store_path, KINDLE *kindle,
                                       size_t size);

extern UTILITY_API void fix_open_time_to_trading_time(KINDLE *k,
                                                      struct tm *ttm);

extern UTILITY_API int convert_kindle(const char *code, KINDLE *kindle,
                                      size_t size, CANDLE_PERIOD_TYPE period,
                                      const char *csvpath,
                                      const char *kindle_path);

extern UTILITY_API int convert_kindle_data(const char *code,
                                           const char *kindle_path);

extern UTILITY_API int get_days_in_month(const struct tm *ttm);
extern UTILITY_API bool over_one_month(const struct tm *ntm, time_t n,
                                       const struct tm *otm, time_t o,
                                       time_t prev);
extern UTILITY_API bool over_period(time_t now, time_t start, struct tm *new_tm,
                                    struct tm *old_tm,
                                    CANDLE_PERIOD_TYPE period);
extern UTILITY_API bool over_one_week(struct tm *new_tm, struct tm *old_tm,
                                      time_t now, time_t start);
extern UTILITY_API bool over_one_day(struct tm *old_tm, time_t now);

extern UTILITY_API int compare_files(const char* file1, const char* file2);




template<typename K, typename T>
void clear_map(std::map<K, T*>& m) {
  for (auto& p : m) {
    if (p.second)
        delete (p.second);
  }
  m.clear();
}

template<typename T>
void clear_vector(std::vector<T*>& v) {
  for (auto&p : v) {
    if (p)
      delete p;
  }
  v.clear();
}

extern UTILITY_API std::vector<std::string> split(const string& input, const string& regex);


#endif // UTILITY_H
