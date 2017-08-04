/*
 * Computation: D = A + B + C
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


#include "clmath/hosts/vectors_cpu.h"
#include "clmath/hosts/vectors_gpu.h"


//------------------------------------------------------------------------------

#define TOL    (0.001)   // tolerance used in floating point comparisons
#define LENGTH (1024)    // length of vectors a, b, c and d


//------------------------------------------------------------------------------


int main(int argc, char** argv)
{
  // cl_int          err;               // error code returned from OpenCL calls

  unsigned int correct;           // number of correct results
  size_t dataSize = sizeof(float) * LENGTH;
  float * h_A = (float *)malloc(dataSize);       // a vector
  float * h_B = (float *)malloc(dataSize);       // b vector
  float * h_C = (float *)malloc(dataSize);       // c vector

  float * h_D_cpu = (float *)malloc(dataSize);       // d vector (result)
  float * h_D_gpu = (float *)malloc(dataSize);
  vector_add_plus_cpu (h_A, h_B, h_C, h_D_cpu, LENGTH);

  // engine * nvidia0 = (engine *)malloc(sizeof(engine));

  // cl_mem d_a;                     // device memory used for the input  a vector
  // cl_mem d_b;                     // device memory used for the input  b vector
  // cl_mem d_c;                     // device memory used for the input c vector
  // cl_mem d_d;                     // device memory used for the output d vector

  // Fill vectors a and b with random float values
  int i = 0;

  // Set up platform and GPU device

  // cl_uint numPlatforms;

  // Find number of platforms
  // err = clGetPlatformIDs(0, NULL, &numPlatforms);
  // checkError(err, "Finding platforms");
  // if (numPlatforms == 0)
  // {
  //   printf("Found 0 platforms!\n");
  //   return EXIT_FAILURE;
  // }

  // Get all platforms
  // cl_platform_id Platform[numPlatforms];
  // err = clGetPlatformIDs(numPlatforms, Platform, NULL);
  // checkError(err, "Getting platforms");

  // Secure a GPU
  // for (i = 0; i < numPlatforms; i++)
  // {
  //   err = clGetDeviceIDs(Platform[i], DEVICE, 1, &nvidia0->device_id, NULL);
  //   if (err == CL_SUCCESS)
  //   {
  //     break;
  //   }
  // }

  // if (nvidia0->device_id == NULL)
  //   checkError(err, "Getting device");

  // err = output_device_info(device_id);
  // checkError(err, "Outputting device info");

 // unsigned int count = LENGTH;
 //  char * kernel_srcs = file_read ("cl/src/kernels/vector_add.cl");
  // engine_init (nvidia0, kernel_srcs);

  // vector_add_plus
  // engine_compute (nvidia0, "vector_add_plus");
  vector_add_plus_gpu (h_A, h_B, h_C, h_D_gpu, LENGTH);


  // Create the input (a, b, e, g) arrays in device memory
  // NB: we copy the host pointers here too
  // d_a  = clCreateBuffer(nvidia0->context,  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  dataSize, h_a, &err);
  // checkError(err, "Creating buffer d_a");
  // d_b  = clCreateBuffer(nvidia0->context,  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  dataSize, h_b, &err);
  // checkError(err, "Creating buffer d_b");
  // d_c  = clCreateBuffer(nvidia0->context,  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  dataSize, h_c, &err);
  // checkError(err, "Creating buffer d_c");

  // Create the output arrays in device memory
  // d_d  = clCreateBuffer(nvidia0->context,  CL_MEM_WRITE_ONLY, dataSize, NULL, &err);
  // checkError(err, "Creating buffer d_d");

  // const int count = LENGTH;

  // Enqueue kernel
  // Set the arguments to our compute kernel
  // err  = clSetKernelArg(nvidia0->kernel, 0, sizeof(cl_mem), &d_a);
  // err |= clSetKernelArg(nvidia0->kernel, 1, sizeof(cl_mem), &d_b);
  // err |= clSetKernelArg(nvidia0->kernel, 2, sizeof(cl_mem), &d_c);
  // err |= clSetKernelArg(nvidia0->kernel, 3, sizeof(cl_mem), &d_d);
  // err |= clSetKernelArg(nvidia0->kernel, 4, sizeof(unsigned int), &count);
  // checkError(err, "Setting kernel arguments");

  // Execute the kernel over the entire range of our 1d input data set
  // letting the OpenCL runtime choose the work-group size
  // global = count;
  // err = clEnqueueNDRangeKernel(nvidia0->commands, nvidia0->kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
  // checkError(err, "Enqueueing kernel 1st time");

  // Read back the result from the compute device
  // err = clEnqueueReadBuffer(nvidia0->commands, d_d, CL_TRUE, 0, sizeof(float) * count, h_d, 0, NULL, NULL );
  // checkError(err, "Reading back d_f");

  // Test the results
  correct = 0;
  float tmp;

  unsigned int count = LENGTH;
  for(i = 0; i < count; i++)
  {
    // tmp = h_a[i] + h_b[i] + h_c[i];     // assign element i of a+b+c to tmp
    tmp = h_D_cpu[i] - h_D_gpu[i];                               // compute deviation of expected and output result
    if(tmp*tmp < TOL*TOL)                        // correct if square deviation is less than tolerance squared
    {
      correct++;
      // printf(" tmp %f h_a %f h_b %f h_c %f h_d %f\n",tmp, h_a[i], h_b[i], h_c[i], h_d[i]);
    }
    else 
      printf(" tmp %f h_a %f h_b %f h_c %f h_d %f\n",tmp, h_A[i], h_B[i], h_C[i], h_D_gpu[i]);
  }

  // summarize results
  printf("D = A+B+C:  %d out of %d results were correct.\n", correct, count);

  // cleanup then shutdown
  // clReleaseMemObject(d_a);
  // clReleaseMemObject(d_b);
  // clReleaseMemObject(d_c);
  // clReleaseMemObject(d_d);
  // engine_cleanup (nvidia0);

  free (h_A);
  free (h_B);
  free (h_C);
  free (h_D_cpu);
  free (h_D_gpu);

  return 0;
}

