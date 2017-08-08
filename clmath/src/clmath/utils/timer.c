#ifdef _OPENMP
  #include <omp.h>
#else
  #include <sys/time.h>
#endif

#include "stdlib.h"

#include "clmath/utils/timer.h"


double wtime ()
{
#ifdef _OPENMP
  return omp_get_wtime();
#else
  static int sec = -1;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  if (sec < 0) sec = tv.tv_sec;
  return (tv.tv_sec - sec) + 1.0e-6 * tv.tv_usec;
#endif
}
