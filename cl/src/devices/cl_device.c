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
  device->index = clReleaseKernel (device->kernel);
  device->index = clReleaseProgram (device->program);

  device->index = clFlush (device->queue);
  device->index = clFinish (device->queue);
  device->index = clReleaseCommandQueue (device->queue);
  device->index = clReleaseContext (device->context);
}


void device_kernel_create (DEVICE * device, 
                           const char ** strings, 
                           const size_t * lengths,
                           const char * kernel_name)
{
  device->program = clCreateProgramWithSource (device->context,
                                               1,
                                               strings,
                                               lengths,
                                               &device->index);

  device->index = clBuildProgram (device->program,
                                  1,
                                  &device->uuid,
                                  NULL,
                                  NULL,
                                  NULL);

  device->kernel = clCreateKernel (device->program, 
                                   kernel_name, 
                                   &device->index);
}


void device_kernel_config (DEVICE * device, const size_t * global_work_size,
                                            const size_t * local_work_size)
{
  device->index = clEnqueueNDRangeKernel (device->queue,
                                          device->kernel,
                                          1,
                                          NULL,
                                          global_work_size,
                                          local_work_size,
                                          0,
                                          NULL,
                                          NULL);
}


void device_kernel_set (DEVICE * device, cl_uint arg_index,
                                         size_t arg_size,
                                         const void * arg_value)
{
  device->index = clSetKernelArg (device->kernel, arg_index, arg_size, arg_value);
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


cl_int device_buffer_read (DEVICE * device, cl_mem device_A, cl_int size, int * host_A)
{
  device->index = clEnqueueReadBuffer (device->queue,
                                       device_A,
                                       CL_TRUE,
                                       0,
                                       size,
                                       host_A,
                                       0,
                                       NULL,
                                       NULL);
}
