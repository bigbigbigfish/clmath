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
  cl_ulong max_constant_buffer_size;

  cl_command_queue_properties queue_properties;
  cl_bool image_support;
  cl_uint max_read_image_args;
  cl_uint max_write_image_args;
  // CL_DEVICE_IMAGE2D_MAX_WIDTH
  // CL_DEVICE_IMAGE2D_MAX_HEIGHT
  // CL_DEVICE_IMAGE3D_MAX_WIDTH
  // CL_DEVICE_IMAGE3D_MAX_HEIGHT
  // CL_DEVICE_IMAGE3D_MAX_DEPTH
  size_t sz_max_dims[5];
  // CL_DEVICE_VECTOR_WIDTH_CHAR
  // CL_DEVICE_VECTOR_WIDTH_SHORT
  // CL_DEVICE_VECTOR_WIDTH_INT
  // CL_DEVICE_VECTOR_WIDTH_LONG
  // CL_DEVICE_VECTOR_WIDTH_FLOAT
  // CL_DEVICE_VECTOR_WIDTH_DOUBLE
  cl_uint vec_width[6];
} DEVICE;


cl_platform_id * platforms_summary (void); 
cl_device_id * devices_summary (cl_platform_id platform_id);
DEVICE get_device_info (cl_device_id id);


void print_device_info (DEVICE device);
void system_info (void);

#endif
