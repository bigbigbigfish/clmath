#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#ifdef __cplusplus
 #include <cstdio>
#endif

#include <stdio.h>

const char *err_code (cl_int err_in);
void check_error(cl_int err, const char *operation, char *filename, int line);

#define checkError(E, S) check_error(E,S,__FILE__,__LINE__)
