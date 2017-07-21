#include "../../include/hosts/filer.h"
#include "../../include/devices/cl_device.h"
#include "../../include/devices/cl_engine.h"

#define MAX_SOURCE_SIZE (0x100000)


void engine_start (DEVICE * device, const size_t * global_work_size,
                                    const size_t * local_work_size,
                                    char * kernelpath)
{
  device_create (device);

  char * kernel_srcs = (char*)malloc(MAX_SOURCE_SIZE);
  size_t source_size = read_file (kernelpath, kernel_srcs, MAX_SOURCE_SIZE);  
  // create kernel program in the device
  device_kernel_create (device, (const char **)&kernel_srcs,
                                (const size_t *)&source_size,
                                "vector_add");
  device_kernel_config (device, global_work_size, local_work_size);

  free (kernel_srcs);
}
