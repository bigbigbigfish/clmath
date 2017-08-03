#include "cl/devices/cl_errors.h"
#include "cl/devices/cl_engines.h"


void engine_init (engine * t, char * kernel_srcs)
{
  cl_int status;

  t->context = clCreateContext (0, 1, &t->device_id, NULL, NULL, &status);
  checkError (status, "Creating context");

  t->commands = clCreateCommandQueue (t->context, t->device_id, 0, &status);
  checkError (status, "Creating command queue");

  t->program = clCreateProgramWithSource (t->context, 1, (const char**)&kernel_srcs, NULL, &status);
  checkError (status, "Creating program");

  // build program
  status = clBuildProgram (t->program, 0, NULL, NULL, NULL, NULL);
  if (status != CL_SUCCESS)
  {
    size_t len;
    char buffer[2048];

    printf ("Error: Failed to build program executable (%s)!\n", err_code(status));
    clGetProgramBuildInfo (t->program, t->device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
    printf ("%s\n", buffer);

    return EXIT_FAILURE;
  }
}


void engine_compute (engine * t, char * kernel_name)
{
  cl_int status;

  t->kernel = clCreateKernel (t->program, kernel_name, &status);
  checkError (status, "Creating kernel");
}


void engine_cleanup (engine * t)
{
  clReleaseProgram (t->program);
  clReleaseKernel (t->kernel);
  clReleaseCommandQueue (t->commands);
  clReleaseContext (t->context);

  free (t);
}
