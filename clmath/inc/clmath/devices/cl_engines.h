#ifndef CL_ENGINES_H
#define CL_ENGINES_H

#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif


typedef struct
{
  cl_device_id device_id;

  cl_context context;
  cl_command_queue commands;

  cl_program program;
  cl_kernel kernel;
} engine;


void engine_init (engine * t, char * kernel_srcs);
void engine_compute (engine * t, char * kernel_name);
void engine_cleanup (engine * t);


#endif
