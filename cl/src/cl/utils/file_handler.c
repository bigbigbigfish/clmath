#include <stdio.h>

#include "cl/utils/file_handler.h"

char * file_read (char * filepath)
{
  char * buffer = NULL;
  int string_size;
  int read_size;
 
  FILE * handler = fopen (filepath, "r");
  if (handler)
  {
    fseek (handler, 0, SEEK_END);
    string_size = ftell (handler);
    rewind (handler);

    buffer = (char*)malloc(sizeof(char)*(string_size+1));
    read_size = fread (buffer, sizeof(char), string_size, handler);
    buffer[string_size] = '\0';

    if (string_size != read_size)
    {
      free (buffer);
      buffer = NULL;
    }

    fclose (handler);
  }

  return buffer;
}
