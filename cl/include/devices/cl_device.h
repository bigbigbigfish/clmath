#ifndef CL_DEVICE_H
#define CL_DEVICE_H

#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif


typedef struct
{
  cl_int index;
  cl_device_id uuid;
  cl_context context;
  cl_command_queue queue;

  cl_program program;
  cl_kernel kernel;
} DEVICE;


// device
void device_create (DEVICE * device);
void device_del (DEVICE * device);

// kernel
void device_kernel_create (DEVICE * device, 
                           const char ** strings, 
                           const size_t * lengths,
                           const char * kernel_name);
void device_kernel_config (DEVICE * device, const size_t * global_work_size,
                                            const size_t * local_work_size);
void device_kernel_set (DEVICE * device, cl_uint arg_index,
                                         size_t arg_size,
                                         const void * arg_value);

// buffer
cl_mem device_buffer_create (DEVICE * device, cl_mem_flags flags, cl_int size);
void device_buffer_del (DEVICE * device, cl_mem device_A);
cl_int device_buffer_write (DEVICE * device, cl_mem device_A, cl_int size, int * host_A);
cl_int device_buffer_read (DEVICE * device, cl_mem device_A, cl_int size, int * host_A);


#endif
