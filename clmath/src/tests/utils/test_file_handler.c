#include <stdio.h>
#include <stdlib.h>

#include "clmath/utils/file_handler.h"

int main (void)
{
  char * kernel = file_read ("clmath/src/clmath/kernels/vector_add.cl");
  if (kernel)
  {
    puts (kernel);
    free (kernel);
  }

  return 0;
}
