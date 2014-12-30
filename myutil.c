#include <mach/mach_time.h>
#include <time.h>
#include <gmp.h>
#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#ifdef __MACH__
#include <sys/time.h>
//clock_gettime is not implemented on OSX
int clock_gettime(struct timespec* t) {
  struct timeval now;
  int rv = gettimeofday(&now, NULL);
  if (rv) return rv;
  t->tv_sec  = now.tv_sec;
  t->tv_nsec = now.tv_usec * 1000;
  return 0;
}
#endif

double mytime (void)
{
  int flag;
  
  struct timespec tp;
  double timing;
  flag = clock_gettime(&tp);
    if (flag == 0) timing = tp.tv_sec + 1.0e-9*tp.tv_nsec;
    else           timing = -17.0;

    return(timing);
};

int mynbits (mpz_t x) {
  return mpz_sizeinbase(x,2);
}
