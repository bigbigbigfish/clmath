#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif

#include <stdio.h>

#define TOL (0.001)
#define LENGTH (1024)

#ifndef DEVICE
  #define DEVICE CL_DEVICE_TYPE_GPU
#endif


int main (void)
{
  cl_int status;

  size_t data_size = sizeof(float) * LENGTH;
  float * h_A = (float*)malloc(data_size);
  float * h_B = (float*)malloc(data_size);
  float * h_C = (float*)malloc(data_size);
  float * h_D = (float*)malloc(data_size);
  float * h_E = (float*)malloc(data_size);
  float * h_F = (float*)malloc(data_size);
  float * h_G = (float*)malloc(data_size);

  unsigned int correct;

  size_t global;
  cl_device_id device_ids[2];
  cl_context context;
  cl_command_queue command;
  cl_program program;
  cl_kernel kernel_vector_add;

  cl_mem d_A;
  cl_mem d_B;
  cl_mem d_C;
  cl_mem d_D;
  cl_mem d_E;
  cl_mem d_F;
  cl_mem d_G;

  // fill vectors A and B with random float values
  int i = 0;
  for (i = 0; i < LENGTH; ++i)
  {
    h_A[i] = rand() / (float)RAND_MAX;
    h_B[i] = rand() / (float)RAND_MAX;
    h_E[i] = rand() / (float)RAND_MAX;
    h_G[i] = rand() / (float)RAND_MAX;
  }

  cl_uint n_platforms;
  status = clGetPlatformIDs (0, NULL, &n_platforms);
  printf ("%d of platforms found.\n", n_platforms);

  cl_platform_id platforms[n_platforms];
  status = clGetPlatformIDs (n_platforms, platforms, NULL);
 
  cl_uint n_devices;
  status = clGetDeviceIDs (platforms[0], CL_DEVICE_TYPE_GPU, 0, NULL, &n_devices);
  printf ("%d of devices found.\n", n_devices);

  status = clGetDeviceIDs (platforms[0], CL_DEVICE_TYPE_GPU, 2, &device_ids, NULL);
  printf ("devices ids: %d, %d\n", device_ids[0], device_ids[1]);

  context = clCreateContext (0, 1, &device_ids[0], NULL, NULL, &status);
  command = clCreateCommandQueue (context, device_ids[0], 0, &status);

  FILE * kernel_srcs = fopen("cl/src/kernels/vector_add.cl", "rb");
  program = clCreateProgramWithSource (context, 1, (const char **)&kernel_srcs, NULL, &status);
  status = clBuildProgram (program, 0, NULL, NULL, NULL, NULL); 
  kernel_vector_add = clCreateKernel (program, "vector_add", &status);

  d_A = clCreateBuffer (context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, data_size, h_A, &status);
  d_B = clCreateBuffer (context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, data_size, h_B, &status);
  d_E = clCreateBuffer (context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, data_size, h_E, &status);
  d_G = clCreateBuffer (context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, data_size, h_G, &status);

  d_C = clCreateBuffer(context, CL_MEM_READ_WRITE, data_size, NULL, &status);
  d_D = clCreateBuffer(context, CL_MEM_READ_WRITE, data_size, NULL, &status);
  d_F = clCreateBuffer(context, CL_MEM_READ_WRITE, data_size, NULL, &status);

  const int count = LENGTH;
  global = count;
  status = clSetKernelArg (kernel_vector_add, 0, sizeof(cl_mem), &d_A);
  status = clSetKernelArg (kernel_vector_add, 1, sizeof(cl_mem), &d_B);
  status = clSetKernelArg (kernel_vector_add, 2, sizeof(cl_mem), &d_C);
  status = clSetKernelArg (kernel_vector_add, 3, sizeof(unsigned int), &count);
  status = clEnqueueNDRangeKernel (command, kernel_vector_add, 1, NULL, &global, NULL, 0, NULL, NULL);

  status = clSetKernelArg (kernel_vector_add, 0, sizeof(cl_mem), &d_E);
  status = clSetKernelArg (kernel_vector_add, 1, sizeof(cl_mem), &d_C);
  status = clSetKernelArg (kernel_vector_add, 2, sizeof(cl_mem), &d_D);
  status = clEnqueueNDRangeKernel (command, kernel_vector_add, 1, NULL, &global, NULL, 0, NULL, NULL);

  status = clEnqueueReadBuffer (command, d_F, CL_TRUE, 0, sizeof(float) * count, h_F, 0, NULL, NULL); 

  // test the result
  correct = 0;
  float tmp;
  for (i = 0; i < count; ++i)
  {
    tmp = h_A[i] + h_B[i] + h_E[i] + h_G[i];
    tmp -= h_F[i];
    if (tmp*tmp < TOL*TOL)
      correct++;
    else
      printf ("tmp %f h_A %f h_B %f h_E %f h_G %f h_F %f\n", tmp, h_A[i], h_B[i], h_E[i], h_G[i], h_F[i]);
  }
  printf ("C = A + B + E + G: %d out of %d results were correct.\n", correct, count);

  // clean up
  clReleaseMemObject (d_A);
  clReleaseMemObject (d_B);
  clReleaseMemObject (d_C);
  clReleaseMemObject (d_D);
  clReleaseMemObject (d_E);
  clReleaseMemObject (d_F);
  clReleaseMemObject (d_G);
  
  clReleaseProgram (program);
  clReleaseKernel (kernel_vector_add);
  clReleaseCommandQueue (command);
  clReleaseContext (context);

  free (h_A);
  free (h_B);
  free (h_C);
  free (h_D);
  free (h_E);
  free (h_F);
  free (h_G);

  return 0;
}
