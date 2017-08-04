#include <stdio.h>
#include <stdlib.h>

#include "clmath/hosts/vectors_gpu.h"


// extern int output_device_info(cl_device_id );

//------------------------------------------------------------------------------

#define TOL    (0.001)   // tolerance used in floating point comparisons
#define LENGTH (1024)    // length of vectors a, b, and c


//------------------------------------------------------------------------------


int main(int argc, char** argv)
{

    size_t dataSize = sizeof(float) * LENGTH;
    float*       h_a = (float *)malloc(dataSize);       // a vector
    float*       h_b = (float *)malloc(dataSize);       // b vector
    float*       h_c = (float *)malloc(dataSize);       // c vector (result)
    float*       h_d = (float *)malloc(dataSize);       // d vector (result)
    float*       h_e = (float *)malloc(dataSize);       // e vector
    float*       h_f = (float *)malloc(dataSize);       // f vector (result)
    float*       h_g = (float *)malloc(dataSize);       // g vector
    unsigned int correct;           // number of correct results



    // Fill vectors a and b with random float values
    int i = 0;
    for(i = 0; i < LENGTH; i++){
        h_a[i] = (float)rand() / (float)RAND_MAX;
        h_b[i] = (float)rand() / (float)RAND_MAX;
        h_e[i] = (float)rand() / (float)RAND_MAX;
        h_g[i] = (float)rand() / (float)RAND_MAX;
    }

    vector_add_gpu (h_a, h_b, h_c, h_d, h_e, h_f, h_g, LENGTH);    

    
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


    free(h_a);
    free(h_b);
    free(h_c);
    free(h_d);
    free(h_e);
    free(h_f);
    free(h_g);

    return 0;
}
