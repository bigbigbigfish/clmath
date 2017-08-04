#include <stdio.h>

#include "clmath/utils/eval.h"


void eval_results (float * result_cpu, float * result_gpu, const unsigned int count)
{
  float tmp;
  unsigned int correct = 0;
  
  for (int i = 0; i < count; ++i)
  {
    tmp = result_cpu[i] - result_gpu[i];
    if (tmp*tmp < TOLERANCE*TOLERANCE)
      correct++;
    else
      printf ("index %d: tmp %f result_cpu %f result_gpu %f\n", i, result_cpu[i], result_gpu[i]);
  }

  printf ("Corrections: %d out of %d results were correct.\n", correct, count);
}
