#include <stdio.h>
#include <stdlib.h>

#include "include/hosts/data.h"
#include "include/hosts/filer.h"
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
  int * host_A = (int*)malloc(sizeof(int)*LIST_SIZE);
  int * host_B = (int*)malloc(sizeof(int)*LIST_SIZE);
  generate_int_data_asc (host_A, LIST_SIZE);
  generate_int_data_desc (host_B, LIST_SIZE);
 
  // Load the kernel source code into the array source_str
  char * kernel_srcs = (char*)malloc(MAX_SOURCE_SIZE);
  size_t source_size = read_file ("cl/src/kernels/vector_add.cl", kernel_srcs, MAX_SOURCE_SIZE);
 
  DEVICE * device = malloc(sizeof(DEVICE));
  device_create (device);
  // create kernel program in the device 
  device_kernel_create (device, (const char **)&kernel_srcs, 
                                (const size_t *)&source_size,
                                "vector_add");
  size_t global_item_size = LIST_SIZE;  // process the entire lists 
  size_t local_item_size = 64;  // divide work items into groups of 64
  device_kernel_config (device, &global_item_size, &local_item_size);

  // copy data from host to device
  // Create memory buffers on the device for each vector
  cl_mem device_A = device_buffer_create (device, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int));
  cl_mem device_B = device_buffer_create (device, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int));
  cl_mem device_C = device_buffer_create (device, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof(int));
  // Copy the lists A and B to their respective memory buffers
  device->index = device_buffer_write (device, device_A, LIST_SIZE * sizeof(int), host_A);
  device->index = device_buffer_write (device, device_B, LIST_SIZE * sizeof(int), host_B);
  // Set the arguments of the kernel
  device_kernel_set (device, 0, sizeof(cl_mem), (void *)&device_A);
  device_kernel_set (device, 1, sizeof(cl_mem), (void *)&device_B);
  device_kernel_set (device, 2, sizeof(cl_mem), (void *)&device_C);
 
  // Read the memory buffer C on the device to the local variable C
  int * host_C = (int*)malloc(sizeof(int)*LIST_SIZE);
  device_buffer_read (device, device_C, LIST_SIZE * sizeof(int), host_C);
 
  // Display the result to the screen
  for(int i = 0; i < LIST_SIZE; i++)
    printf("%d + %d = %d\n", host_A[i], host_B[i], host_C[i]);
 
  // Clean up
  device->index = clReleaseMemObject(device_A);
  device->index = clReleaseMemObject(device_B);
  device->index = clReleaseMemObject(device_C);
  device_del (device);

  free(host_A);
  free(host_B);
  free(host_C);

  return 0;
}
