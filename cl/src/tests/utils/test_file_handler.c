#include "cl/utils/file_handler.h"

int main ()
{
  char * kernel = file_read ("cl/src/kernels/vector_add.cl");
  if (kernel)
  {
    puts (kernel);
    free (kernel);
  }

  return 0;
}
