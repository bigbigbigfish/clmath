#include "../../include/devices/cl_device.h"


void create_device (DEVICE * device)
{
  // get platforms
  cl_platform_id platform_id = NULL;
  cl_uint num_platforms;
  cl_int platform = clGetPlatformIDs (1, &platform_id, &num_platforms);
 
  // get device
  device->uuid = NULL;
  cl_uint num_devices;
  device->index = clGetDeviceIDs (platform_id,
                               CL_DEVICE_TYPE_DEFAULT,
                               1,
                               &device->uuid,
                               &num_devices);
  
  // create context
  device->context = clCreateContext (NULL, 1, &device->uuid, NULL, NULL, &device->index);
  // create a command queue
  device->queue = clCreateCommandQueue (device->context, device->uuid, 0, &device->index);
}


void del_device (DEVICE * device)
{
  device->index = clFlush (device->queue);
  device->index = clFinish (device->queue);
  device->index = clReleaseCommandQueue (device->queue);
  device->index = clReleaseContext (device->context);
}

