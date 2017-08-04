#include <stdio.h>


#include "clmath/devices/cl_errors.h"
#include "clmath/devices/cl_engines.h"


void engine_init (engine * t, char * kernel_srcs)
{
  cl_int status;

  cl_uint n_platforms;
  status = clGetPlatformIDs (0, NULL, &n_platforms);
  checkError (status, "Finding platforms");
  if (n_platforms == 0)
  {
    printf ("0 platforms found.\n");
    // return EXIT_FAILURE;
  }

  cl_platform_id platforms[n_platforms];
  status = clGetPlatformIDs (n_platforms, platforms, NULL);
  checkError (status, "Getting platforms");
   
  for (int i = 0; i < n_platforms; ++i)
  {
    status = clGetDeviceIDs (platforms[i], CL_DEVICE_TYPE_DEFAULT, 1, &t->device_id, NULL);
    if (status == CL_SUCCESS)
    {
      break;
    }
  }
 
  if (t->device_id == NULL)
    checkError (status, "Gettings device"); 

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

    // return EXIT_FAILURE;
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
