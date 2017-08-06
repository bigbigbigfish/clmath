#include "clmath/utils/file_handler.h"
#include "clmath/devices/cl_engines.h"
#include "clmath/devices/cl_errors.h"

#include "clmath/hosts/matrix_gpu.h"


void matrix_mul_gpu (float * h_A,
                     float * h_B,
                     float * h_C,
                     const unsigned int M,
                     const unsigned int N,
                     const unsigned int K)
{
  cl_int status;
  cl_mem d_A;
  cl_mem d_B;
  cl_mem d_C;

  engine * t = (engine*)malloc(sizeof(engine));
  char * kernel_srcs = file_read ("clmath/src/clmath/kernels/matrix.cl");
  engine_init (t, kernel_srcs);
  engine_compute (t, "matrix_mul");

  d_A = clCreateBuffer (t->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*M*K, h_A, &status);
  checkError (status, "Creating buffer d_A");
  d_B = clCreateBuffer (t->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*K*N, h_B, &status);
  checkError (status, "Creating buffer d_B");
  d_C = clCreateBuffer (t->context, CL_MEM_WRITE_ONLY, sizeof(float)*M*N, h_C, &status);
  checkError (status, "Creating buffer d_C");

  const size_t global[2] = {M, N};
  const size_t count = M*N;
  for (int i = 0; i < M*N; ++i)
  {
    status = clSetKernelArg (t->kernel, 0, sizeof(cl_mem), &d_A);
    status |= clSetKernelArg (t->kernel, 1, sizeof(cl_mem), &d_B);
    status |= clSetKernelArg (t->kernel, 2, sizeof(cl_mem), &d_C);
    status |= clSetKernelArg (t->kernel, 3, sizeof(unsigned int), &count);
    checkError (status, "Setting kernel argumenets");

    status = clEnqueueNDRangeKernel (t->commands, t->kernel, 2, NULL, global, NULL, 0, NULL, NULL);
    checkError (status, "Enqueuing kernel");

    status = clFinish (t->commands);

    status = clEnqueueReadBuffer (t->commands, d_C, CL_TRUE, 0, sizeof(float)*M*N, h_C, 0, NULL, NULL);
    checkError (status, "Reading back buffer d_C");
  }

  clReleaseMemObject (d_A);
  clReleaseMemObject (d_B);
  clReleaseMemObject (d_C);
  engine_cleanup (t);
}
