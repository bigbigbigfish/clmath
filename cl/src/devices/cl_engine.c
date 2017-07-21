#include "../../include/hosts/filer.h"
#include "../../include/devices/cl_device.h"
#include "../../include/devices/cl_engine.h"

#define MAX_SOURCE_SIZE (0x100000)


void engine_start (DEVICE * device, const size_t * global_work_size,
                                    const size_t * local_work_size,
                                    char * kernelpath,
                                    const char * kernelname)
{
  device_create (device);

  char * kernel_srcs = (char*)malloc(MAX_SOURCE_SIZE);
  size_t source_size = read_file (kernelpath, kernel_srcs, MAX_SOURCE_SIZE);  
  // create kernel program in the device
  device_kernel_create (device, (const char **)&kernel_srcs,
                                (const size_t *)&source_size,
                                kernelname);
  device_kernel_config (device, global_work_size, local_work_size);

  free (kernel_srcs);
}


void engine_close (DEVICE * device)
{
  device_del (device);
}


void engine_compute (DEVICE * device)
{

}
