#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  struct tm ttm;
  time_t t = time(0);
  time_t t2 = 0, t3 = 0;
 
  localtime_r(&t, &ttm);

  printf("%d\n", ttm.tm_wday);

  t3 = t - ttm.tm_wday * 24 * 60 * 60;

  ttm.tm_wday = 3;

  t2 = mktime(&ttm);
  
  printf("%lu, %lu, %lu\n", t, t2, t3);

  return 0;
}

