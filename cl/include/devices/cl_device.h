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
} DEVICE;


void create_device (DEVICE * device);
void del_device (DEVICE * device);


#endif
