#include "cl/devices/cl_errors.h"
#include "cl/devices/cl_engines.h"


void engine_init (engine * t)
{
  cl_int status;

  t->context = clCreateContext (0, 1, &t->device_id, NULL, NULL, &status);
  checkError (status, "Creating context");

  t->commands = clCreateCommandQueue (t->context, t->device_id, 0, &status);
  checkError (status, "Creating command queue");
}


void engine_cleanup (engine * t)
{
  clReleaseProgram (t->program);
  clReleaseKernel (t->kernel);
  clReleaseCommandQueue (t->commands);
  clReleaseContext (t->context);

  free (t);
}
