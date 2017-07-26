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

  // platforms 
  cl_platform_id * platform_ids = platforms_summary ();

  // (platform) intel: device
  cl_device_id * intel_device_ids = devices_summary (platform_ids[0]);
  DEVICE intel_device = get_device_info (intel_device_ids[0]);
  print_device_info (intel_device);

  // (platform) nvidia: devices
  cl_device_id * nvidia_device_ids = devices_summary (platform_ids[1]);
  DEVICE nvidia_device_0 = get_device_info (nvidia_device_ids[0]);
  print_device_info (nvidia_device_0);
  DEVICE nvidia_device_1 = get_device_info (nvidia_device_ids[1]);
  print_device_info (nvidia_device_1);
  system_info ();

  free (platform_ids);
  free (intel_device_ids);
  free (nvidia_device_ids);

  return 0;
}
