#include <stdio.h>
#include <stdlib.h>


size_t read_file (char * filepath, char * srcs, size_t src_max_size)
{
  FILE * fp = fopen (filepath, "r");
  if (!fp)
  {
    fprintf (stderr, "Failed to load kernel.\n");
    exit (EXIT_FAILURE);
  }

  size_t srcs_size = fread (srcs, 1, src_max_size, fp);

  fclose (fp);
}
