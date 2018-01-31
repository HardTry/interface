#ifndef GOMMEM_H
#define GOMMEM_H


#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"

#include "ctpif_global.h"
#include "utility.h"
#include "ctpif_data.h"
#include "ctpifio.h"

typedef CThostFtdcDepthMarketDataField CtpMarketData;
typedef vector<string> CtpServers;
typedef vector<string> CtpInstruments;

typedef TThostFtdcDateType DateType;

/// struct for CTP Parameters
typedef struct CtpParam {
  string dif_name;           /// data-interface name, must be "ctp"
  string ctpmd_logpath;      /// ctp marketing data log path
  string brokerid;           /// broker id
  string clientid;           /// client id
  string password;           /// password
  bool udp;                  /// using udp protocol
  CtpServers mduri;          /// URI of Market Data servers
  CtpServers tduri;          /// URI of Trade servers
  string inst_file;          /// instruments list file
  CtpInstruments instrument; /// instruments
  string shred_mem_name;     /// shared memrory name
} CtpParam;


typedef struct GmdParam {
  uint32_t max_tick_per_sec;  /// max amount of tick data per second
  uint32_t max_time_long;     /// max trading duringtion in one day, in second
  uint32_t tick_buffer_num;   /// amout number of tick buffer
  uint32_t save_tickbuf_time; /// interval time for save tick buffer to
                              /// disk(persistent), in minutes

  char gmdlog[MAX_PATH];          /// the log filepath for go-markting-data
  char gtdlog[MAX_PATH];          /// the log filepath for trade
  char data_store_path[MAX_PATH]; /// persistent all data, include kindle data,
                                  /// etc.
  char cal_store_path[MAX_PATH];  /// calculate result for MA, MACD or something
                                  /// etc.
  char tick_data_path[MAX_PATH];  /// buffer for tick_data_path
  char data_rtpath[MAX_PATH];     /// realtime CTPMMD struct for calculate or
                                  /// analysis
  uint32_t save_loop_interval;    /// the interval time for save tick data loop

  string exit_time; /// exit time

  CtpParam* ctp_param;
} GmdParam;

typedef enum CTPMD_STATUS {
  CTPMD_STATUS_DISCONNECTED = -2,
  CTPMD_STATUS_NO_STATUS = -1,
  CTPMD_STATUS_CONNECTED = 0,
  CTPMD_STATUS_USER_LOGIN,
  CTPMD_STATUS_USER_LOGOUT,
  CTPMD_STATUS_HEARTBEAT,
  CTPMD_STATUS_ERROR,
  CTPMD_STATUS_SUBSCRIPT,
  CTPMD_STATUS_UNSUBSCRIPT,
  CTPMD_STATUS_DEPTH_MD,
  CTPMD_STATUS_UNSUB_QUOTERSP,
  CTPMD_STATUS_SUB_QUOTERSP,
  CTPMD_STATUS_QUOTERSP,
} CTPMD_STATUS;

typedef struct CtpStatus {
    CTPMD_STATUS status;
    uint32_t data;
} CtpStatus;

typedef struct CTPMMD {
  TThostFtdcPriceType LastPrice;
  TThostFtdcPriceType BidPrice1;
  TThostFtdcPriceType AskPrice1;
  // TThostFtdcMoneyType Turnover;
  TThostFtdcLargeVolumeType OpenInterest;
  time_t UpdateTime;
  TThostFtdcVolumeType Volume;
  TThostFtdcVolumeType BidVolume1;
  TThostFtdcVolumeType AskVolume1;
  TThostFtdcMillisecType UpdateMillisec;
} CTPMMD;

typedef struct CTPMD {
  ftime_t key;
  PriceType LastPrice;
  PriceType BidPrice1;
  PriceType AskPrice1;
  VolumeType Volume;
  VolumeType BidVolume1;
  VolumeType AskVolume1;
  VolumeType OpenInterest;
  MoneyType Turnover;
} CTPMD;



/**
 * @brief parse_ctp_json parse json config for ctp
 * @param doc the document of json file
 * @return the result
 */
// extern int CTPIF_API parse_gmd_config(rapidjson::Document &doc);


extern CTPIF_API bool invalid_data(CtpMarketData& data);
/**
 * @brief convert_ctpmd_2_ctpmmd convert the CtpMarketData format to CTPMMD
 */
extern void CTPIF_API convert_ctpmd_2_ctpmmd(CTPMMD *mmd, CtpMarketData *data);
extern void CTPIF_API convert_ctpmd_2_ctpmd(DATAPTR md, DATAPTR data);


/**
 * @brief tb_2_ctpmmd convete TB-tick-data to CTPMMD format
 * @param tbcsv the TB-tick-data file
 * @param mmdfile the CTPMMD file
 * @return zero for good
 */
extern CTPIF_API int tb_2_ctpmmd(const char *tbcsv, const char *mmdfile);

extern CTPIF_API int sample_tick_data(const char *tick_csv,
                                        const char *mdfile, const char *csvfile,
                                        const int interval);


/*
{
  "name": "ctp - 1",
  "brokerid": "9999",
  "clientid": "068576",
  "password": "simnow",
  "udp": false,
  "tduri": "tcp://180.168.146.187:10000"
}
*/

/// struct for CTP Parameters
typedef struct CtpSpeedParam {
  string name;           /// data-interface name, must be "ctp"
  string brokerid;           /// broker id
  string clientid;           /// client id
  string password;           /// password
  bool udp;                  /// using udp protocol
  string uri;                /// URI of Trade servers
} CtpSpeedParam;


/*
"name": "ShangHai",
"instrument": "rb1801",
"price": 10,
"dir": 1
*/

typedef struct SpeedTestCase {
  string name;
  string instrument;
  double price;
  int dir;
  int volume;
}SpeedTestCase;


typedef struct CtpSpeedTest {
  string logpath;
  int    interval;
  int    loop_times;
  std::vector<CtpSpeedParam*> params;
  std::vector<SpeedTestCase*> cases;
}CtpSpeedTest;


#endif // GOMMEM_H
