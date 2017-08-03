#ifndef CL_ENGINES_H
#define CL_ENGINES_H

typedef struct
{
  cl_device_id device_id;

  cl_context context;
  cl_command_queue commands;

  cl_program program;
  cl_kernel kernel;  
} engine;

#endif
