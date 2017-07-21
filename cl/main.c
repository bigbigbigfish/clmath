#include <stdio.h>
#include <stdlib.h>

#include "hosts/data.h"
#include "hosts/filer.h"
#include "include/devices/cl_device.h"


#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif

#define MAX_SOURCE_SIZE (0x100000)

int main(void) 
{
  // Create the two input vectors
  const int LIST_SIZE = 1024;
  int *A = (int*)malloc(sizeof(int)*LIST_SIZE);
  int *B = (int*)malloc(sizeof(int)*LIST_SIZE);
  generate_int_data_asc (A, LIST_SIZE);
  generate_int_data_desc (B, LIST_SIZE);
 
  // Load the kernel source code into the array source_str
  char * kernel_srcs = (char*)malloc(MAX_SOURCE_SIZE);
  size_t source_size = read_file ("cl/kernels/vector_add.cl", kernel_srcs, MAX_SOURCE_SIZE);
 
  DEVICE * device = malloc(sizeof(DEVICE));
  create_device(device);

  // Create memory buffers on the device for each vector 
  cl_mem a_mem_obj = clCreateBuffer(device->context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &device->index);
  cl_mem b_mem_obj = clCreateBuffer(device->context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &device->index);
  cl_mem c_mem_obj = clCreateBuffer(device->context, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof(int), NULL, &device->index);
 
  // Copy the lists A and B to their respective memory buffers
  device->index = clEnqueueWriteBuffer(device->queue, a_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
  device->index = clEnqueueWriteBuffer(device->queue, b_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), B, 0, NULL, NULL);
 
  // Create a program from the kernel source
  cl_program program = clCreateProgramWithSource(device->context, 1, (const char **)&kernel_srcs, (const size_t *)&source_size, &device->index);
 
  // Build the program
  device->index = clBuildProgram(program, 1, &device->uuid, NULL, NULL, NULL);
 
  // Create the OpenCL kernel
  cl_kernel kernel = clCreateKernel(program, "vector_add", &device->index);
 
  // Set the arguments of the kernel
  device->index = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
  device->index = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
  device->index = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);
 
  // Execute the OpenCL kernel on the list
  size_t global_item_size = LIST_SIZE; // Process the entire lists
  size_t local_item_size = 64; // Divide work items into groups of 64
  device->index = clEnqueueNDRangeKernel(device->queue, kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);
 
  // Read the memory buffer C on the device to the local variable C
  int *C = (int*)malloc(sizeof(int)*LIST_SIZE);
  device->index = clEnqueueReadBuffer(device->queue, c_mem_obj, CL_TRUE, 0, 
            LIST_SIZE * sizeof(int), C, 0, NULL, NULL);
 
  // Display the result to the screen
  for(int i = 0; i < LIST_SIZE; i++)
    printf("%d + %d = %d\n", A[i], B[i], C[i]);
 
  // Clean up
  device->index = clReleaseKernel(kernel);
  device->index = clReleaseProgram(program);
  device->index = clReleaseMemObject(a_mem_obj);
  device->index = clReleaseMemObject(b_mem_obj);
  device->index = clReleaseMemObject(c_mem_obj);

  del_device (device);

  free(A);
  free(B);
  free(C);

  return 0;
}
