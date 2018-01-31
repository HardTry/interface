#include "fatcatserver.h"


#define process_udp_package_callback int(const char* buf, int len, const char* sender_ip)


static void release_interest_changed(vecInterestChanged& vic) {
  for (auto& p : vic)
    if (p) delete p;
  vic.clear();
}

FatcatServer::FatcatServer()
{
  Callback<process_udp_package_callback>::func
          = std::bind(&FatcatServer::process_package,
                      this,
                      std::placeholders::_1,
                      std::placeholders::_2,
                      std::placeholders::_3);
  process_package_callback ppcb = static_cast<process_package_callback>(
                  Callback<process_udp_package_callback>::callback);
  set_process_callback(ppcb);
}

FatcatServer::~FatcatServer() {
  release_interest_changed(vic_);
}

int FatcatServer::process_package(const char* buf, int len, const char* sender_ip) {
  (void)sender_ip;

  if (!(len % sizeof(InterestChanged))) {
    InterestChanged*  ptr = (InterestChanged*)buf;
    for (int i = 0; i < len / (int)(sizeof(InterestChanged)); ++i, ++ptr) {
      InterestChanged* ic = new InterestChanged();
      memcpy(ic, ptr, sizeof(InterestChanged));
      vic_.emplace_back(ic);
    }
  }
  return 0;
}
