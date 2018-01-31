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
  int last_pos = -1;
  int total = 0;
} DataPosChange;


typedef int (*function_new_ctp_data) (const char* instrument, int spos, int epos);

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
          int curpos = dfCtpmd->got_new_data();
          if (curpos >= 0 && p.second.last_pos < curpos) {
            if (fnewdata_)
              fnewdata_(p.first.c_str(), p.second.last_pos, curpos);
            else
              printf("Got %s data from %d to %d\n", p.first.c_str(), p.second.last_pos, curpos);
            p.second.last_pos = curpos;
          }
        }
      }
    }

    void set_get_new_data_function(function_new_ctp_data fnewdata) {
      fnewdata_ = fnewdata;
    }

protected:
    int cur_buf_index_ = 0;
    int prev_buf_index_ = 0;
    std::map<std::string, DataPosChange> mapPos_;
    GmdParam* param_ = nullptr;
    char TradingDay_[20] = {""};
    function_new_ctp_data fnewdata_ = nullptr;
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
            int curpos = dfCtpmd->got_new_data();
            if (curpos >= 0 && p.second.last_pos < curpos) {
              printf("Got %s data from %d to %d\n", p.first.c_str(), p.second.last_pos, curpos);
              save_one_inst_ctpmd_(dfCtpmd, p.first, p.second, curpos);
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
          printf("Error write %s %d %d\n", inst.c_str(), dpc.last_pos + 1, curpos);
          return -2;
        }
        fclose(fp);
        dpc.total += curpos - dpc.last_pos;
        printf("Wrote %d ~ %d, %d, total %d, ctpmd data, %s, %s\n", dpc.last_pos + 1,
               curpos, curpos - dpc.last_pos, dpc.total, inst.c_str(), TradingDay_);
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
    set_converter(convert_ctpmd_2_ctpmd);
  }

private:
  GmdParam* param_ = nullptr;
};


extern CTPIF_API void test_read_thread(void* r);
extern CTPIF_API void test_save_thread(void* r);
extern CTPIF_API void test_write_thread(void* w, CtpMarketData* data, CtpMarketDataInfo* info,
                              size_t max_size, std::chrono::milliseconds loop_interval);

#endif // CTPIF_DATASAVER_H
