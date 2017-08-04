#ifdef __APPLE__
  #include <OpenCL/cl.h>
#else
  #include <CL/cl.h>
#endif


#include "cl/utils/file_handler.h"
#include "cl/hosts/vectors_gpu.h"
#include "cl/devices/cl_errors.h"


void vector_add_plus_gpu (engine * t,
                          float * h_A,
                          float * h_B,
                          float * h_C,
                          float * h_D,
                          const unsigned int count)
{

  cl_int status;
  size_t global = count;

  cl_mem d_A;
  cl_mem d_B;
  cl_mem d_C;
  cl_mem d_D;

  d_A = clCreateBuffer (t->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, count, h_A, &status);
  checkError (status, "Creating buffer d_A");
  d_B = clCreateBuffer (t->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, count, h_B, &status);
  checkError (status, "Creating buffer d_B");
  d_C = clCreateBuffer (t->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, count, h_C, &status);
  checkError (status, "Creating buffer d_C");
  d_D = clCreateBuffer (t->context, CL_MEM_WRITE_ONLY, count, NULL, &status);
  checkError (status, "Creating buffer d_D");

  status = clSetKernelArg (t->kernel, 0, sizeof(cl_mem), &d_A);
  status |= clSetKernelArg (t->kernel, 1, sizeof(cl_mem), &d_B);
  status |= clSetKernelArg (t->kernel, 2, sizeof(cl_mem), &d_C); 
  status |= clSetKernelArg (t->kernel, 3, sizeof(cl_mem), &d_D); 
  status |= clSetKernelArg (t->kernel, 4, sizeof(unsigned int), &count);
  checkError (status, "Setting kernel arguments");

  status = clEnqueueNDRangeKernel (t->commands, t->kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
  checkError (status, "Enqueueing kernel 1st time");

  // status = clEnqueueReadBuffer (t->commands, d_D, CL_TRUE, 0, sizeof(float)*count, h_D, 0, NULL, NULL);
  // checkError (status, "Reading back d_D");

  clReleaseMemObject (d_A);
  clReleaseMemObject (d_B);
  clReleaseMemObject (d_C);
  clReleaseMemObject (d_D);
}

