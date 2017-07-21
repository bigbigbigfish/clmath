#include "../../include/devices/cl_device.h"


void device_create (DEVICE * device)
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


void device_del (DEVICE * device)
{
  device->index = clReleaseProgram (device->program);

  device->index = clFlush (device->queue);
  device->index = clFinish (device->queue);
  device->index = clReleaseCommandQueue (device->queue);
  device->index = clReleaseContext (device->context);
}


void device_program_create (DEVICE * device, const char ** strings, const size_t * lengths)
{
  device->program = clCreateProgramWithSource (device->context,
                                               1,
                                               strings,
                                               lengths,
                                               &device->index);
}


cl_mem device_buffer_create (DEVICE * device, cl_mem_flags flags, cl_int size)
{
  cl_mem device_A = clCreateBuffer (device->context,
                                    flags,
                                    size,
                                    NULL,
                                    &device->index);
  return device_A;
}


cl_int device_buffer_write (DEVICE * device,
                            cl_mem device_A, 
                            cl_int size,
                            int * host_A)
{
  device->index = clEnqueueWriteBuffer (device->queue,
                                        device_A,
                                        CL_TRUE,
                                        0,
                                        size,
                                        host_A,
                                        0,
                                        NULL,
                                        NULL);
  return device->index;
}
