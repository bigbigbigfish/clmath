#include <stdio.h>
#include <stdbool.h>

#include "../include/devices/cl_devices.h"

#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif


int main ()
{
 
  cl_platform_id * platform_ids = platforms_summary ();
  cl_device_id * intel_device_ids = devices_summary (platform_ids[0]);
  cl_device_id * nvidia_device_ids = devices_summary (platform_ids[1]);
  system_info ();

  free (platform_ids);
  free (intel_device_ids);
  free (nvidia_device_ids);

  return 0;
}
