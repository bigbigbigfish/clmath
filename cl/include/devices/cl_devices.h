#ifndef CL_DEVICES_H
#define CL_DEVICES_H

#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif


typedef struct
{
  char name[1024];
  char vendor[1024];
  char driver_version[1024];

  cl_device_type cl_type;
  cl_uint compute_units;

  size_t workitem_dims;
  size_t workitem_size[3];
  size_t workgroup_size;

  cl_uint clock_frequency;
  cl_uint addr_bits;
  cl_ulong max_mem_alloc_size;
  cl_ulong global_mem_size;
  cl_bool error_correction_support;
  cl_device_local_mem_type local_mem_type;
  cl_ulong local_mem_size;
} DEVICE;

cl_platform_id * platforms_summary (void); 
cl_device_id * devices_summary (cl_platform_id platform_id);

DEVICE get_device_info (cl_device_id id);

void system_info (void);

#endif
