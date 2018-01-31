#ifndef FATCATSERVER_H
#define FATCATSERVER_H

#include "utility_global.h"
#include "udp_server.h"
#include "ctpif_global.h"
#include "ctptdspi.h"
#include <map>
#include <set>
#include <vector>

#define INSTRUMENT_LENGTH 10

typedef struct Contract {
    int32_t  dir = 0;
    char     instrument[INSTRUMENT_LENGTH] = {""};

    Contract() {}
    Contract(int32_t d, const char* inst) : dir(d) {
      strcpy_s(instrument, INSTRUMENT_LENGTH, inst);
    }

    bool operator< (const struct Contract& interest) const {
      int c = strcmp(instrument, interest.instrument);
      if (c == 0) {
        return (dir < interest.dir);
      } else {
        return (c < 0);
      }
    }

    bool operator== (const struct Contract& interest) const {
      return ((dir == interest.dir)
               && (!strcmp(instrument, interest.instrument)));
    }
} Contract;

typedef struct HoldInterests {
    uint32_t account;
    char     exch[3];
    Contract interest;
    uint32_t amount;
} HoldInterests;


enum INTEREST_CHANGE_TYPE {
  INTEREST_CHANGE_TYPE_NOCHANGE,
  INTEREST_CHANGE_TYPE_ADD,
  INTEREST_CHANGE_TYPE_MINUS,
  INTEREST_CHANGE_TYPE_CLOSED,
  INTEREST_CHANGE_TYPE_NEW,
};


typedef struct InterestChanged {
  Contract interest;
  time_t tt = 0;
  uint32_t account = 0;
  INTEREST_CHANGE_TYPE type = INTEREST_CHANGE_TYPE_NOCHANGE;
  int32_t changed_vol = 0;
  int32_t before = 0;
  int32_t after = 0;
} InterestChanged;


typedef std::vector<HoldInterests*> vecInterest;
typedef std::map<std::string, HoldInterests*> mapStr2Interest;

typedef std::multimap<Contract, HoldInterests*> mmapI2I;
typedef std::map<Contract, HoldInterests*> mapI2I;
typedef std::map<Contract, uint32_t> mapInterestAmount;
typedef std::map<uint32_t, mapI2I*> mapA2I2I;

typedef std::set<Contract> setInterest;
typedef std::set<uint32_t> setAccount;

typedef std::vector<InterestChanged*> vecInterestChanged;

class FatcatServer : public UdpSrvReceiver
{
public:
    FatcatServer();
    virtual ~FatcatServer();
    void set_ctp_trade_spi(CtpTradeSpi* spi) {spi_ = spi;}
protected:
    int process_package(const char* buf, int len, const char* sender_ip);
private:
    CtpTradeSpi* spi_ = nullptr;
    vecInterestChanged vic_;
};



#endif // FATCATSERVER_H
