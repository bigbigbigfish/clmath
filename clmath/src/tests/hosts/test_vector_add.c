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
  unsigned int correct;           // number of correct results
  size_t dataSize = sizeof(float) * LENGTH;
  float * h_A = (float *)malloc(dataSize);       // a vector
  float * h_B = (float *)malloc(dataSize);       // b vector
  float * h_C = (float *)malloc(dataSize);       // c vector

  float * h_D_cpu = (float *)malloc(dataSize);       // d vector (result)
  float * h_D_gpu = (float *)malloc(dataSize);
  vector_add_plus_cpu (h_A, h_B, h_C, h_D_cpu, LENGTH);

  int i = 0;


  
  vector_add_plus_gpu (h_A, h_B, h_C, h_D_gpu, LENGTH);



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


  free (h_A);
  free (h_B);
  free (h_C);
  free (h_D_cpu);
  free (h_D_gpu);

  return 0;
}

