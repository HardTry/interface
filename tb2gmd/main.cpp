#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "str.h"
#include "ctpif.h"

/*
-rw-rw-r--. 1 sean sean 23433010 Oct 10 01:23 ag888_1m.csv
-rw-rw-r--. 1 sean sean 27370082 Oct 10 01:21 al888_1m.csv
-rw-rw-r--. 1 sean sean 36676078 Oct 10 01:22 au888_1m.csv
-rw-rw-r--. 1 sean sean  9892613 Oct 10 02:49 bu888_1m.csv
-rw-rw-r--. 1 sean sean 25576854 Oct 10 03:00 CF888_1m.csv
-rw-rw-r--. 1 sean sean 30996733 Oct 10 01:24 cu888_1m.csv
-rw-rw-r--. 1 sean sean  8047023 Oct 10 01:25 fu888_1m.csv
-rw-rw-r--. 1 sean sean 12153009 Oct 10 02:51 hc888_1m.csv
-rw-rw-r--. 1 sean sean 10912398 Oct 10 02:59 i9888_1m.csv
-rw-rw-r--. 1 sean sean  5494772 Oct 10 03:17 IC888_1m.csv
-rw-rw-r--. 1 sean sean 21992652 Oct 10 03:10 IF888_1m.csv
-rw-rw-r--. 1 sean sean  5518315 Oct 10 03:15 IH888_1m.csv
-rw-rw-r--. 1 sean sean 18080494 Oct 10 02:57 j9888_1m.csv
-rw-rw-r--. 1 sean sean 11652140 Oct 10 02:57 jm888_1m.csv
-rw-rw-r--. 1 sean sean 25570112 Oct 10 02:58 m9888_1m.csv
-rw-rw-r--. 1 sean sean  8757555 Oct 10 02:56 ni888_1m.csv
-rw-rw-r--. 1 sean sean 11402292 Oct 10 01:28 pb888_1m.csv
-rw-rw-r--. 1 sean sean 24164233 Oct 10 01:29 rb888_1m.csv
-rw-rw-r--. 1 sean sean 25709526 Oct 10 01:33 ru888_1m.csv
-rw-rw-r--. 1 sean sean  6510296 Oct 10 02:53 sn888_1m.csv
-rw-rw-r--. 1 sean sean 24520771 Oct 10 03:01 SR888_1m.csv
-rw-rw-r--. 1 sean sean  4925137 Oct 10 03:13 T9888_1m.csv
-rw-rw-r--. 1 sean sean  9462719 Oct 10 03:11 TF888_1m.csv
-rw-rw-r--. 1 sean sean  2260308 Oct 10 02:48 wr888_1m.csv
-rw-rw-r--. 1 sean sean 30264262 Oct 10 02:49 zn888_1m.csv



-rw-rw-r--. 1 sean sean 23563655 Oct 16 04:54 ag888_1m.csv
-rw-rw-r--. 1 sean sean 36819845 Oct 16 04:53 au888_1m.csv
-rw-rw-r--. 1 sean sean 25672957 Oct 16 11:05 CF888_1m.csv
-rw-rw-r--. 1 sean sean 11003652 Oct 16 11:00 i9888_1m.csv
-rw-rw-r--. 1 sean sean 22053930 Oct 16 11:50 IF888_1m.csv
-rw-rw-r--. 1 sean sean 18175891 Oct 16 10:33 j9888_1m.csv
-rw-rw-r--. 1 sean sean 25661841 Oct 16 11:34 m9888_1m.csv
-rw-rw-r--. 1 sean sean  8876741 Oct 16 11:23 ni888_1m.csv
-rw-rw-r--. 1 sean sean 26777911 Oct 16 11:30 p9888_1m.csv
-rw-rw-r--. 1 sean sean 24249433 Oct 16 05:36 rb888_1m.csv
-rw-rw-r--. 1 sean sean 25798374 Oct 16 05:14 ru888_1m.csv
-rw-rw-r--. 1 sean sean 24609620 Oct 16 11:08 SR888_1m.csv


./tb2gmd m ag888 /work/znl/gom/data/kindle/data-1m/ag888_1m.csv /work/znl/gom/data/cal/ag888.m1
./tb2gmd m au888 /work/znl/gom/data/kindle/data-1m/au888_1m.csv /work/znl/gom/data/cal/au888.m1
./tb2gmd m CF888 /work/znl/gom/data/kindle/data-1m/CF888_1m.csv /work/znl/gom/data/cal/CF888.m1
./tb2gmd m i9888 /work/znl/gom/data/kindle/data-1m/i9888_1m.csv /work/znl/gom/data/cal/i9888.m1
./tb2gmd m IF888 /work/znl/gom/data/kindle/data-1m/IF888_1m.csv /work/znl/gom/data/cal/IF888.m1
./tb2gmd m j9888 /work/znl/gom/data/kindle/data-1m/j9888_1m.csv /work/znl/gom/data/cal/j9888.m1
./tb2gmd m m9888 /work/znl/gom/data/kindle/data-1m/m9888_1m.csv /work/znl/gom/data/cal/m9888.m1
./tb2gmd m ni888 /work/znl/gom/data/kindle/data-1m/ni888_1m.csv /work/znl/gom/data/cal/ni888.m1
./tb2gmd m p9888 /work/znl/gom/data/kindle/data-1m/p9888_1m.csv /work/znl/gom/data/cal/p9888.m1
./tb2gmd m rb888 /work/znl/gom/data/kindle/data-1m/rb888_1m.csv /work/znl/gom/data/cal/rb888.m1
./tb2gmd m ru888 /work/znl/gom/data/kindle/data-1m/ru888_1m.csv /work/znl/gom/data/cal/ru888.m1
./tb2gmd m SR888 /work/znl/gom/data/kindle/data-1m/SR888_1m.csv /work/znl/gom/data/cal/SR888.m1



./tb2gmd k ag888 /work/znl/gom/data/cal
./tb2gmd k au888 /work/znl/gom/data/cal
./tb2gmd k CF888 /work/znl/gom/data/cal
./tb2gmd k i9888 /work/znl/gom/data/cal
./tb2gmd k IF888 /work/znl/gom/data/cal
./tb2gmd k j9888 /work/znl/gom/data/cal
./tb2gmd k m9888 /work/znl/gom/data/cal
./tb2gmd k ni888 /work/znl/gom/data/cal
./tb2gmd k p9888 /work/znl/gom/data/cal
./tb2gmd k rb888 /work/znl/gom/data/cal
./tb2gmd k ru888 /work/znl/gom/data/cal
./tb2gmd k SR888 /work/znl/gom/data/cal
*/

int main(int argc, char *argv[]) {
  if (argc != 4 && argc != 5) {
    printf("Usage: tb2gmd t <in: tb tick csv file> <out: ctpmmd file>\n");
    printf("Usage: tb2gmd m <instrument code> <in: tb tick csv file> <out: ctpmmd file>\n");
    printf("Usage: tb2gmd k <instrument code> <kindle file store path>\n");
    printf("Usage: tb2gmd s <in: tb tick csv file> <out: ctpmmd file> <out: csv file>\n");
    return (0);
  }

  size_t amount = 0;
  if (!strcmp(argv[1], "t")) //tick data
    return tb_2_ctpmmd(argv[2], argv[3]);
  else if (!strcmp(argv[1], "m")) //minute data
    return tb_2_min(argv[2], argv[3], argv[4], &amount);
  else if (!strcmp(argv[1], "k")) //minute data
    return convert_kindle_data(argv[2], argv[3]);
  else if (!strcmp(argv[1], "s"))
    return sample_tick_data(argv[2], argv[3], argv[4], 30);

  return (0);
}
