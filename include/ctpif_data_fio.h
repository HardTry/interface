#ifndef CTPIF_DATASAVER_H
#define CTPIF_DATASAVER_H

#include "ctpif_global.h"
#include "ctpif.h"
#include "ctpmdspi.h"
#include "ctptdspi.h"
#include "mem.h"
#include "str.h"
#include "utility.h"
#include <chrono>
#include <stdio.h>
#include <thread>

typedef struct DataPosChange {
  int64_t last_pos = -1;
  int64_t last_total = 0;
} DataPosChange;


typedef int (*got_new_ctp_data_cb) (const char* instrument, int64_t cur_count, int64_t spos, int64_t epos);

class CTPIF_API CtpDataReader : public CtpReader {
public:
    CtpDataReader(GmdParam* param)
        : CtpReader(param->ctp_param->instrument, false, true)
        , param_(param)
    {
      for(auto& inst : param->ctp_param->instrument) {
        DataPosChange dpc;
        mapPos_.insert(std::make_pair(inst, dpc));
      }

      initMemory( param->max_tick_per_sec * param->save_tickbuf_time * 60,
                  param->max_time_long * param->max_tick_per_sec,
                  param->tick_buffer_num,
                  param->data_rtpath,
                  param->ctp_param->shred_mem_name.c_str());
    }

    virtual ~CtpDataReader() {}

    bool write_buf_was_changed() {
      if (ibiptr_->cur_tick_buf_ != cur_buf_index_) {
        prev_buf_index_ = cur_buf_index_;
        cur_buf_index_ = ibiptr_->cur_tick_buf_;
        return true;
      }

      return false;
    }

    bool writer_is_exited() {
      if (ibiptr_)
        return ibiptr_->is_exited_;
      else
        return true;
    }

    void got_new_data() {
      for(auto& p : mapPos_) {
        TSDATAFRAME(CTPMD) *dfCtpmd =
          reinterpret_cast<TSDATAFRAME(CTPMD) *>(maps2p_[p.first]);
        if (dfCtpmd) {
          int64_t count = dfCtpmd->get_data_amount();
          int64_t curpos = dfCtpmd->get_cur_pos();
          if (count > p.second.last_total && curpos >= 0 && p.second.last_pos <= curpos) {
            if (fnewdata_)
              fnewdata_(p.first.c_str(), count, p.second.last_pos, curpos);
            else
              printf("Got %s data from %ld to %ld\n", p.first.c_str(), p.second.last_pos, curpos);
            p.second.last_pos = curpos;
            p.second.last_total = count;
          }
        }
      }
    }

    void set_get_new_data_function(got_new_ctp_data_cb fnewdata) {
      fnewdata_ = fnewdata;
    }

protected:
    int cur_buf_index_ = 0;
    int prev_buf_index_ = 0;
    std::map<std::string, DataPosChange> mapPos_;
    GmdParam* param_ = nullptr;
    char TradingDay_[20] = {""};
    got_new_ctp_data_cb fnewdata_ = nullptr;
};



class CTPIF_API CtpDataSaver : public CtpDataReader {
public:
    CtpDataSaver(GmdParam* param) : CtpDataReader(param) {}
    virtual ~CtpDataSaver() {}

    int save(bool last_buffer = false) {
      int ret = save_raw_data_buffer_(last_buffer);
      if (!ret)
        save_ctpmd_();
      return ret;
    }

private:
    int save_raw_data_buffer_(bool last_buffer = false) {
      for (auto& inst: param_->ctp_param->instrument) {
        int64_t data_size = 0;
        int index = ((last_buffer) ? cur_buf_index_ :prev_buf_index_);
        CtpMarketData* ticks = raw_data_set_[index]->get_dataptr(&data_size);
        if (!ticks) return -1;

        char filepath[MAX_PATH];
        snprintf(filepath, MAX_PATH, "%s/%s-%s.tick", param_->tick_data_path, inst.c_str(), ticks->TradingDay);
        FILE* fp = fopen(filepath, "ab");
        if (!fp) return -2;

        int64_t j = 0;
        int64_t amount = (last_buffer) ? raw_data_set_[index]->info->di.pos + 1: data_size;
        for (int64_t i = 0; i < amount; ++i) {
          if (!strcmp(ticks[i].InstrumentID, inst.c_str())) {
            ++j;
            if (1 != fwrite(ticks + i, sizeof(CtpMarketData), 1, fp)) {
              fclose(fp);
              printf("Error on write tick %s, %s", ticks[i].InstrumentID, ticks[i].TradingDay);
              return -3;
            }
          }
        }
        fclose(fp);
        printf("Wrote %ld tick data, %s, %s\n", j, inst.c_str(), ticks->TradingDay);
      }
      return 0;
    }

    int save_ctpmd_() {
        for(auto& p : mapPos_) {
          TSDATAFRAME(CTPMD) *dfCtpmd =
            reinterpret_cast<TSDATAFRAME(CTPMD) *>(maps2p_[p.first]);
          if (dfCtpmd) {
            int64_t count = dfCtpmd->get_data_amount();
            int64_t curpos = dfCtpmd->get_cur_pos();
            if (count > p.second.last_total && curpos >= 0 && p.second.last_pos < curpos) {
              // printf("Got %s data from %d to %d\n", p.first.c_str(), p.second.last_pos, curpos);
              save_one_inst_ctpmd_(dfCtpmd, p.first, p.second, curpos);
              p.second.last_total = count;
              p.second.last_pos = curpos;
            }
          }
        }


        return 0;
    }

    int save_one_inst_ctpmd_(TSDATAFRAME(CTPMD) *dfCtpmd, const std::string& inst, DataPosChange& dpc, int curpos) {
        char filepath[MAX_PATH];
        snprintf(filepath, MAX_PATH, "%s/%s-%s.ctpmd", param_->data_rtpath, inst.c_str(), TradingDay_);
        FILE* fp = fopen(filepath, "ab");
        if (!fp) return -1;
        int64_t s = 0;
        if ((size_t)(curpos - dpc.last_pos) != fwrite(dfCtpmd->get_dataptr(&s) + dpc.last_pos + 1,
                                                      sizeof(CTPMD), curpos - dpc.last_pos, fp)) {
          fclose(fp);
          printf("Error write %s %ld %d\n", inst.c_str(), dpc.last_pos + 1, curpos);
          return -2;
        }
        fclose(fp);
        printf("Wrote %ld ~ %d, %ld, total %ld, ctpmd data, %s, %s\n", dpc.last_pos + 1,
               curpos, curpos - dpc.last_pos, dpc.last_total, inst.c_str(), TradingDay_);
      return 0;
    }
};

class CTPIF_API CtpDataWriter : public CtpWriter {
public:
  CtpDataWriter(GmdParam* param)
    : CtpWriter(param->ctp_param->instrument)
    , param_(param)
  {
    initMemory( param->max_tick_per_sec * param->save_tickbuf_time * 60,
                param->max_time_long * param->max_tick_per_sec,
                param->tick_buffer_num,
                param->data_rtpath,
                param->ctp_param->shred_mem_name.c_str());
    set_converter(ctp_2_candle);
    set_isnew(its_new_candle_data);
    set_merge(merge_candle_data);
  }

private:
  GmdParam* param_ = nullptr;
};


extern CTPIF_API void test_read_thread(void* r);
extern CTPIF_API void test_save_thread(void* r);
extern CTPIF_API void test_write_thread(void* w, CtpMarketData* data, CtpMarketDataInfo* info,
                              size_t max_size, std::chrono::milliseconds loop_interval);


extern CTPIF_API void test_write_candle_thread(void* w, CtpMarketData* data, CtpMarketDataInfo* info,
                              size_t max_size, std::chrono::milliseconds loop_interval);
extern CTPIF_API void test_read_candle_thread(void* r);
extern CTPIF_API void test_save_candle_thread(void* r);
extern CTPIF_API std::ostream& operator << (std::ostream& out, const NR::CandleBar& bar);


class CTPIF_API CtpCandleWriter : public CandleWriter {
public:
  CtpCandleWriter(GmdParam* param)
    : CandleWriter(param->ctp_param->instrument, true, true)
    , param_(param) {
      initMemory( param->max_tick_per_sec * param->save_tickbuf_time * 60,
                  param->max_time_long * param->max_tick_per_sec,
                  param->tick_buffer_num,
                  param->data_rtpath,
                  param->ctp_param->shred_mem_name.c_str());
      set_converter(ctp_2_candle);
      set_isnew(its_new_candle_data);
      set_merge(merge_candle_data);
  }
private:
  GmdParam* param_ = nullptr;
};



class CTPIF_API CtpCandleReader : public CandleReader {
public:
    CtpCandleReader(GmdParam* param)
        : CandleReader(param->ctp_param->instrument, false, true)
        , param_(param)
    {
      for(auto& inst : param->ctp_param->instrument) {
        DataPosChange dpc;
        mapPos_.insert(std::make_pair(inst, dpc));
      }

      initMemory( param->max_tick_per_sec * param->save_tickbuf_time * 60,
                  param->max_time_long * param->max_tick_per_sec,
                  param->tick_buffer_num,
                  param->data_rtpath,
                  param->ctp_param->shred_mem_name.c_str());
    }

    virtual ~CtpCandleReader() {}

    bool write_buf_was_changed() {
      if (ibiptr_->cur_tick_buf_ != cur_buf_index_) {
        prev_buf_index_ = cur_buf_index_;
        cur_buf_index_ = ibiptr_->cur_tick_buf_;
        return true;
      }

      return false;
    }

    bool writer_is_exited() {
      if (ibiptr_)
        return ibiptr_->is_exited_;
      else
        return true;
    }

    void got_new_data() {
      for(auto& p : mapPos_) {
        TSDATAFRAME(NR::CandleBar) *dfCandle =
          reinterpret_cast<TSDATAFRAME(NR::CandleBar) *>(maps2p_[p.first]);
        if (dfCandle) {
          int64_t count = dfCandle->get_data_amount();
          int64_t curpos = dfCandle->get_cur_pos();
          if (count > p.second.last_total || (curpos >= 0 && p.second.last_pos < curpos)) {
            if (fnewdata_)
              fnewdata_(p.first.c_str(), count, p.second.last_pos, curpos);
            else {
              if (curpos >= 0 && p.second.last_pos < curpos)
                printf("Got %s candle data from %ld to %ld\n", p.first.c_str(), p.second.last_pos, curpos);
              else if (count > p.second.last_total && p.second.last_pos == curpos)
                printf("Update candle data %s %ld\n", p.first.c_str(), p.second.last_pos);
            }
            p.second.last_pos = curpos;
            p.second.last_total = count;
          }
        }
      }
    }

    void set_get_new_data_db(got_new_ctp_data_cb fnewdata) {
      fnewdata_ = fnewdata;
    }

    void delete_param() {
      if (nullptr != param_)
        releaseConfig(param_);
    }

    std::map<std::string, DataPosChange>& get_map_pos() { return mapPos_; }
    MapStr2Ptr& get_map_inst_ptr() {return maps2p_;}

    GmdParam* get_param() {return param_;}

    void set_trade_day(const char* td) { strcpy_s(TradingDay_, 19, td); }

protected:
    int cur_buf_index_ = 0;
    int prev_buf_index_ = 0;
    std::map<std::string, DataPosChange> mapPos_;
    GmdParam* param_ = nullptr;
    char TradingDay_[20] = {""};
    got_new_ctp_data_cb fnewdata_ = nullptr;
};



class CTPIF_API CtpCandleSaver : public CtpCandleReader {
public:
  CtpCandleSaver(GmdParam* param) : CtpCandleReader(param) {}
  virtual ~CtpCandleSaver() {}

  void enable_csv(bool encsv = true) {save_to_csv_ = encsv;}

  int save(bool last_buffer = false) {
    int ret = save_raw_data_buffer_(last_buffer);
    if (!ret)
      save_candle_();
    return ret;
  }

private:
  bool save_to_csv_ = true;

  int save_raw_data_buffer_(bool last_buffer = false) {
    printf("save_raw_data_buffer_\n");
    for (auto& inst: param_->ctp_param->instrument) {
      int64_t data_size = 0;
      int index = ((last_buffer) ? cur_buf_index_ :prev_buf_index_);
      CtpMarketData* ticks = raw_data_set_[index]->get_dataptr(&data_size);
      if (!ticks) {
        printf("can not get raw_data_set_[%d]->get_dataptr(&data_size)\n", index);
        return -1;
      }

      char filepath[MAX_PATH];
      snprintf(filepath, MAX_PATH, "%s/%s-%s.tick", param_->tick_data_path, inst.c_str(), ticks->TradingDay);
      printf("save_file %s\n", filepath);
      FILE* fp = fopen(filepath, "ab");
      if (!fp) {
        printf("save_file open error %d\n", errno);
        return -2;
      }

      int64_t j = 0;
      int64_t amount = (last_buffer) ? raw_data_set_[index]->info->di.pos + 1: data_size;
      for (int64_t i = 0; i < amount; ++i) {
        if (!strcmp(ticks[i].InstrumentID, inst.c_str())) {
          ++j;
          if (1 != fwrite(ticks + i, sizeof(CtpMarketData), 1, fp)) {
            fclose(fp);
            printf("Error on write tick %ld, %s, %s", i, ticks[i].InstrumentID, ticks[i].TradingDay);
            return -3;
          }
        }
      }
      fclose(fp);
      printf("Wrote %ld tick data, %s, %s\n", j, inst.c_str(), ticks->TradingDay);
    }
    return 0;
  }

  int save_candle_() {
    printf("save_candle_\n");
    for(auto& p : mapPos_) {
      TSDATAFRAME(NR::CandleBar) *dfCandle =
        reinterpret_cast<TSDATAFRAME(NR::CandleBar)*>(maps2p_[p.first]);
      if (dfCandle) {
        int64_t count = dfCandle->get_data_amount();
        int64_t curpos = dfCandle->get_cur_pos();
        printf("save_candle_ > count %ld, curpos %ld, %s, Last : total %ld, pos %ld\n",
               count, curpos, p.first.c_str(), p.second.last_total, p.second.last_pos);
        if (count > p.second.last_total && curpos >= 0 && p.second.last_pos < curpos) {
          save_inst_candle_(dfCandle, p.first, p.second, curpos);
          printf("save_candle_ > Saved %s candle data from %ld to %ld\n", p.first.c_str(), p.second.last_pos, curpos);
          p.second.last_total = count;
          p.second.last_pos = curpos;
        }
      } else {
        printf("save_candle_ > Can not get dfCandle pointer %s\n", p.first.c_str());
      }
    }


    return 0;
  }

  int save_inst_candle_(TSDATAFRAME(NR::CandleBar)* dfCandle, const std::string& inst, DataPosChange& dpc, int curpos) {
    printf("save_inst_candle_\n");
    int ret = save_candle_binary_(dfCandle, inst, dpc, curpos);

    if (!ret && save_to_csv_)
      ret = save_candle_csv_(dfCandle, inst, dpc, curpos);

    return ret;
  }

  int save_candle_csv_(TSDATAFRAME(NR::CandleBar)* dfCandle, const std::string& inst, DataPosChange& dpc, int curpos) {
    printf("save_candle_csv_ 0, save %s candle csv from %ld to %d\n", inst.c_str(), dpc.last_pos, curpos);
    char filepath[MAX_PATH];
    FILE* fp = nullptr;

    int64_t s = 0;
    NR::CandleBar* ptr = (NR::CandleBar*)(dfCandle->get_dataptr(&s)) + dpc.last_pos + 1;
    printf("save_candle_csv_ 1\n");

    uint32_t day, date, dtime;
    struct tm lt;

    time_t t = (time_t)(ptr->key);
    localtime_s(&lt, &t);
    day = (lt.tm_year + 1900) * 10000 + (lt.tm_mon + 1) * 100 + lt.tm_mday;
    snprintf(filepath, MAX_PATH, "%s/%s-ctp10s-%d.csv", param_->data_rtpath, inst.c_str(), day);
    printf("save_inst_candle_ 2 day %d, file %s\n", day, filepath);

    fp = fopen(filepath, "a+");
    if (fp == nullptr) {
      printf("save_inst_candle_ 2-1 open error %d\n", errno);
      return -11;
    }

    for (int64_t i = 0; i < curpos - dpc.last_pos; ++i, ++ptr) {
      time_t t = (time_t)(ptr->key);
      localtime_s(&lt, &t);
      date = (lt.tm_year + 1900) * 10000 + (lt.tm_mon + 1) * 100 + lt.tm_mday;
      //date was changed
      if (date != day) {
        printf("save_inst_candle_ 3, date was changed, day %d, date %d\n", day, date);
        fclose(fp);
        fp = nullptr;

        day = date;
        snprintf(filepath, MAX_PATH, "%s/%s-ctp10s-%d.csv", param_->data_rtpath, inst.c_str(), day);
        fp = fopen(filepath, "a+");
        printf("save_inst_candle_ 4, day %d, file %s\n", day, filepath);
        if (fp == nullptr) {
          printf("save_inst_candle_ 4-1, open error %d\n", errno);
          return -13;
        }
      }


      dtime = lt.tm_hour * 10000 + lt.tm_min * 100 + lt.tm_sec;
      // fprintf(stdout, "%lf, %ld > %d-%d-%d %d:%d:%d > %u %u > ",
      //         ptr->key, t,
      //         lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday,
      //         lt.tm_hour, lt.tm_min, lt.tm_sec,
      //         date, dtime);
      //
      // fprintf(stdout, "%d,%06d,%.04lf,%.02lf,%.02lf,%.02lf,%.02lf,%.02lf,%.02lf\n",
      //                  date, dtime, ptr->key, ptr->Open, ptr->High, ptr->Low, ptr->Close,
      //                  ptr->Volume, ptr->openInterest);

      if (0 > fprintf(fp, "%d,%06d,%.04lf,%.02lf,%.02lf,%.02lf,%.02lf,%.02lf,%.02lf\n",
                       date, dtime, ptr->key, ptr->Open, ptr->High, ptr->Low, ptr->Close,
                       ptr->Volume, ptr->openInterest)) {
        fclose(fp);
        printf ("save_inst_candle_ 5, %ld, Error wrote to csv %s\n", i, filepath);
        return -12;
      }
    }

    fclose(fp);
    printf("save_inst_candle_ 6, Wrote CSV %ld ~ %d, %ld, total %ld, ctpmd data, %s, %s\n", dpc.last_pos + 1,
           curpos, curpos - dpc.last_pos, dpc.last_total, inst.c_str(), TradingDay_);
    return 0;
  }

  int save_candle_binary_(TSDATAFRAME(NR::CandleBar)* dfCandle, const std::string& inst, DataPosChange& dpc, int curpos) {
    printf("save_candle_binary_ 0\n");
    char filepath[MAX_PATH];
    int64_t s = 0;
    auto ptr = (NR::CandleBar*)(dfCandle->get_dataptr(&s) + dpc.last_pos + 1);
    printf("save_candle_binary_ 1\n");

    time_t t = (time_t)(ptr->key);
    struct tm tt;
    localtime_s(&tt, &t);
    uint32_t day = (tt.tm_year + 1900) * 10000 + (tt.tm_mon + 1) * 100 + tt.tm_mday;

    snprintf(filepath, MAX_PATH, "%s/%s-ctp10s-%d.bin", param_->data_rtpath, inst.c_str(), day);
    printf("save_candle_binary_ 2 %s\n", filepath);
            ;
    FILE* fp = fopen(filepath, "ab");
    if (!fp) {
      printf("save_candle_binary_ 2-1 oopen error %d\n", errno);
      return -1;
    }

    for (int64_t i = 0; i < curpos - dpc.last_pos; ++i, ++ptr) {
      t = (time_t)(ptr->key);
      localtime_s(&tt, &t);
      uint32_t date = (tt.tm_year + 1900) * 10000 + (tt.tm_mon + 1) * 100 + tt.tm_mday;

      if (day != date) {
        printf("save_candle_binary_ change day %d to %d\n", day, date);
        fclose(fp);
        fp = nullptr;
        day = date;

        snprintf(filepath, MAX_PATH, "%s/%s-ctp10s-%d.bin", param_->data_rtpath, inst.c_str(), day);
        printf("save_candle_binary_ 100 Changed to %s\n", filepath);
        fp = fopen(filepath, "ab");
        if (!fp) {
          printf("save_candle_binary_ 100-1 oopen error %d\n", errno);
          return -3;
        }
      }

      if (1 != fwrite(ptr, sizeof(NR::CandleBar), 1, fp)) {
        fclose(fp);
        printf("save_candle_binary_ Error write %ld %s : %ld %d\n", i, inst.c_str(), dpc.last_pos + 1, curpos);
        return -2;
      }
    }

    fclose(fp);
    printf("save_candle_binary_ Wrote Bin %ld ~ %d, %ld, total %ld, ctpmd data, %s, %s\n", dpc.last_pos + 1,
           curpos, curpos - dpc.last_pos, dpc.last_total, inst.c_str(), TradingDay_);
    return 0;
  }
};



#endif // CTPIF_DATASAVER_H
