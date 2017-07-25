#include "stdio.h"

#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif


int main (void)
{
  return 0;
}
