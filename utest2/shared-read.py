#!/bin/python
from ctypes import *

'''
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

typedef struct KINDLE {
  time_t UpdateTime;
  PriceType open;
  PriceType high;
  PriceType low;
  PriceType close;
  Volume vol;
  LargeVolume interest;
} KINDLE;

'''

class CTPMMD(Structure):
  _fields_ = [
    ("LastPrice", c_double),
    ("BidPrice", c_double),
    ("AskPrice", c_double),
    ("OpenInterest", c_double),
    ("UpdateTime", c_ulonglong),
    ("Volume", c_int),
    ("BidVolume1", c_int),
    ("AskVolume1", c_int),
    ("UpdateMillisec", c_int)]


print "CTPMMD OK!"

print "KINDLE OK!"
