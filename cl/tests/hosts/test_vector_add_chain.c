#include <stdio.h>
#include <stdlib.h>

#include "cl/hosts/vectors_gpu.h"


// extern int output_device_info(cl_device_id );

//------------------------------------------------------------------------------

#define TOL    (0.001)   // tolerance used in floating point comparisons
#define LENGTH (1024)    // length of vectors a, b, and c


//------------------------------------------------------------------------------


int main(int argc, char** argv)
{
    cl_int          err;               // error code returned from OpenCL calls

    size_t dataSize = sizeof(float) * LENGTH;
    float*       h_a = (float *)malloc(dataSize);       // a vector
    float*       h_b = (float *)malloc(dataSize);       // b vector
    float*       h_c = (float *)malloc(dataSize);       // c vector (result)
    float*       h_d = (float *)malloc(dataSize);       // d vector (result)
    float*       h_e = (float *)malloc(dataSize);       // e vector
    float*       h_f = (float *)malloc(dataSize);       // f vector (result)
    float*       h_g = (float *)malloc(dataSize);       // g vector
    unsigned int correct;           // number of correct results

    size_t global;                  // global domain size

    // engine * nvidia0 = (engine*)malloc(sizeof(engine));

    cl_mem d_a;                     // device memory used for the input  a vector
    cl_mem d_b;                     // device memory used for the input  b vector
    cl_mem d_c;                     // device memory used for the output c vector
    cl_mem d_d;                     // device memory used for the output d vector
    cl_mem d_e;                     // device memory used for the input e vector
    cl_mem d_f;                     // device memory used for the output f vector
    cl_mem d_g;                     // device memory used for the input g vector

    // Fill vectors a and b with random float values
    int i = 0;
    for(i = 0; i < LENGTH; i++){
        h_a[i] = (float)rand() / (float)RAND_MAX;
        h_b[i] = (float)rand() / (float)RAND_MAX;
        h_e[i] = (float)rand() / (float)RAND_MAX;
        h_g[i] = (float)rand() / (float)RAND_MAX;
    }

    // Set up platform and GPU device

    // cl_uint numPlatforms;

    // Find number of platforms
    // err = clGetPlatformIDs(0, NULL, &numPlatforms);
    // checkError(err, "Finding platforms");
    // if (numPlatforms == 0)
    // {
    //     printf("Found 0 platforms!\n");
    //     return EXIT_FAILURE;
    // }

    // Get all platforms
    // cl_platform_id Platform[numPlatforms];
    // err = clGetPlatformIDs(numPlatforms, Platform, NULL);
    // checkError(err, "Getting platforms");

    // Secure a GPU
    // for (i = 0; i < numPlatforms; i++)
    // {
    //     err = clGetDeviceIDs(Platform[i], DEVICE, 1, &nvidia0->device_id, NULL);
    //     if (err == CL_SUCCESS)
    //     {
    //         break;
    //     }
    // }

    // if (nvidia0->device_id == NULL)
    //     checkError(err, "Getting device");

    // err = output_device_info(device_id);
    // checkError(err, "Outputting device info");
  

    // char * kernel_srcs = file_read ("cl/src/kernels/vector_add.cl");
    // engine_init (nvidia0, kernel_srcs);

    // engine_compute (nvidia0, "vector_add");
    vector_add_gpu (h_a, h_b, h_c, h_d, h_e, h_f, h_g, LENGTH);    

/*
    // Create the input (a, b, e, g) arrays in device memory
    // NB: we copy the host pointers here too
    d_a  = clCreateBuffer(nvidia0->context,  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  dataSize, h_a, &err);
    checkError(err, "Creating buffer d_a");
    d_b  = clCreateBuffer(nvidia0->context,  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  dataSize, h_b, &err);
    checkError(err, "Creating buffer d_b");
    d_e  = clCreateBuffer(nvidia0->context,  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  dataSize, h_e, &err);
    checkError(err, "Creating buffer d_e");
    d_g  = clCreateBuffer(nvidia0->context,  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  dataSize, h_g, &err);
    checkError(err, "Creating buffer d_g");
    
    // Create the output arrays in device memory
    d_c  = clCreateBuffer(nvidia0->context,  CL_MEM_READ_WRITE, dataSize, NULL, &err);
    checkError(err, "Creating buffer d_c");
    d_d  = clCreateBuffer(nvidia0->context,  CL_MEM_READ_WRITE, dataSize, NULL, &err);
    checkError(err, "Creating buffer d_d");
    d_f  = clCreateBuffer(nvidia0->context,  CL_MEM_WRITE_ONLY, dataSize, NULL, &err);
    checkError(err, "Creating buffer d_f"); 

    const int count = LENGTH;

    // Enqueue kernel - first time
    // Set the arguments to our compute kernel
    err  = clSetKernelArg(nvidia0->kernel, 0, sizeof(cl_mem), &d_a);
    err |= clSetKernelArg(nvidia0->kernel, 1, sizeof(cl_mem), &d_b);
    err |= clSetKernelArg(nvidia0->kernel, 2, sizeof(cl_mem), &d_c);
    err |= clSetKernelArg(nvidia0->kernel, 3, sizeof(unsigned int), &count);
    checkError(err, "Setting kernel arguments"); 
	
    // Execute the kernel over the entire range of our 1d input data set
    // letting the OpenCL runtime choose the work-group size
    global = count;
    err = clEnqueueNDRangeKernel(nvidia0->commands, nvidia0->kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
    checkError(err, "Enqueueing kernel 1st time");

    // Enqueue kernel - second time
    // Set different arguments to our compute kernel
    err  = clSetKernelArg(nvidia0->kernel, 0, sizeof(cl_mem), &d_e);
    err |= clSetKernelArg(nvidia0->kernel, 1, sizeof(cl_mem), &d_c);
    err |= clSetKernelArg(nvidia0->kernel, 2, sizeof(cl_mem), &d_d);
    checkError(err, "Setting kernel arguments");
    
    // Enqueue the kernel again    
    err = clEnqueueNDRangeKernel(nvidia0->commands, nvidia0->kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
    checkError(err, "Enqueueing kernel 2nd time");

    // Enqueue kernel - third time
    // Set different (again) arguments to our compute kernel
    err  = clSetKernelArg(nvidia0->kernel, 0, sizeof(cl_mem), &d_g);
    err |= clSetKernelArg(nvidia0->kernel, 1, sizeof(cl_mem), &d_d);
    err |= clSetKernelArg(nvidia0->kernel, 2, sizeof(cl_mem), &d_f);
    checkError(err, "Setting kernel arguments");

    // Enqueue the kernel again    
    err = clEnqueueNDRangeKernel(nvidia0->commands, nvidia0->kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
    checkError(err, "Enqueueing kernel 3rd time");

    // Read back the result from the compute device
    err = clEnqueueReadBuffer( nvidia0->commands, d_f, CL_TRUE, 0, sizeof(float) * count, h_f, 0, NULL, NULL );  
    checkError(err, "Reading back d_f");
*/
    
    // Test the results
    correct = 0;
    float tmp;
    unsigned int count = LENGTH;
    for(i = 0; i < count; i++)
    {
        tmp = h_a[i] + h_b[i] + h_e[i] + h_g[i];     // assign element i of a+b+e+g to tmp
        tmp -= h_f[i];                               // compute deviation of expected and output result
        if(tmp*tmp < TOL*TOL)                        // correct if square deviation is less than tolerance squared
            correct++;
        else {
            printf(" tmp %f h_a %f h_b %f h_e %f h_g %f h_f %f\n",tmp, h_a[i], h_b[i], h_e[i], h_g[i], h_f[i]);
        }
    }

    // summarize results
    printf("C = A+B+E+G:  %d out of %d results were correct.\n", correct, count);

    // cleanup then shutdown
    // clReleaseMemObject(d_a);
    // clReleaseMemObject(d_b);
    // clReleaseMemObject(d_c);
    // clReleaseMemObject(d_d);
    // clReleaseMemObject(d_e);
    // clReleaseMemObject(d_f);
    // clReleaseMemObject(d_g);
    // engine_cleanup (nvidia0);

    free(h_a);
    free(h_b);
    free(h_c);
    free(h_d);
    free(h_e);
    free(h_f);
    free(h_g);

    return 0;
}
