#include <stdio.h>
#include <stdlib.h>

#include "../include/hosts/data.h"
#include "../include/devices/cl_device.h"
#include "../include/devices/cl_engine.h"


int main(void) 
{
  // Create the two input vectors
  const int LIST_SIZE = 1024;
  int * host_A = (int*)malloc(sizeof(int)*LIST_SIZE);
  int * host_B = (int*)malloc(sizeof(int)*LIST_SIZE);
  int * host_C = (int*)malloc(sizeof(int)*LIST_SIZE);
  generate_int_data_asc (host_A, LIST_SIZE);
  generate_int_data_desc (host_B, LIST_SIZE);
 
 
  DEVICE * device = malloc(sizeof(DEVICE));
  size_t global_item_size = LIST_SIZE;  // process the entire lists 
  size_t local_item_size = 64;  // divide work items into groups of 64
  char * kernelpath = "cl/src/kernels/vector_add.cl";
  char * kernelname = "vector_add";
  engine_start (device, &global_item_size, &local_item_size, kernelpath, kernelname);


  // copy data from host to device
  // Create memory buffers on the device for each vector
  cl_mem device_A = device_buffer_create (device, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int));
  cl_mem device_B = device_buffer_create (device, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int));
  cl_mem device_C = device_buffer_create (device, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof(int));
  // copy the lists A and B to their respective memory buffers
  device->index = device_buffer_write (device, device_A, LIST_SIZE * sizeof(int), host_A);
  device->index = device_buffer_write (device, device_B, LIST_SIZE * sizeof(int), host_B);
  // set the arguments of the kernel
  device_kernel_set (device, 0, sizeof(cl_mem), (void *)&device_A);
  device_kernel_set (device, 1, sizeof(cl_mem), (void *)&device_B);
  device_kernel_set (device, 2, sizeof(cl_mem), (void *)&device_C);
  // read the memory buffer C on the device to the local variable C
  device_buffer_read (device, device_C, LIST_SIZE * sizeof(int), host_C);
 
  // display the result to the screen
  for(int i = 0; i < LIST_SIZE; i++)
    printf("%d + %d = %d\n", host_A[i], host_B[i], host_C[i]);
 
  // clean up: device
  device_buffer_del (device, device_A);
  device_buffer_del (device, device_B);
  device_buffer_del (device, device_C);
  engine_close (device);

  // clean up: host
  free(host_A);
  free(host_B);
  free(host_C);

  return 0;
}
