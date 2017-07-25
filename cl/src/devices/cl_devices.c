#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#include "../../include/devices/cl_devices.h"



cl_platform_id * platforms_summary (void)
{
  bool is_platforms = true;
  char platform_name[1024];
  char platform_version[1024];

  cl_uint n_platforms;
  cl_platform_id * platform_ids;
  cl_platform_id selected_platform_id = NULL;
  cl_int platform_status;

  printf ("Platforms Summary\n");
  platform_status = clGetPlatformIDs (0, NULL, &n_platforms);
  if (platform_status != CL_SUCCESS)
  {
    printf ("Platform Status: %i\n", platform_status);
    is_platforms = false;
  }
  else
  {
    if (n_platforms == 0)
    {
      printf ("Platform Status: No OpenCL Platform found!\n");
      is_platforms = false;
    }
    else
    {
      // if n_platforms >= 1
      platform_ids = (cl_platform_id*)malloc(n_platforms * sizeof(cl_platform_id));
      if (platform_ids == NULL)
      {
        printf ("Platform Status: Failed to allocate memory for platform IDs.\n");
        is_platforms = false;
      }

      printf ("Platform Status: %d OpenCL platforms founds.\n", n_platforms);
      platform_status = clGetPlatformIDs (n_platforms, platform_ids, NULL);
      for (cl_uint i = 0; i < n_platforms; ++i)
      {
        platform_status = clGetPlatformInfo (platform_ids[i], CL_PLATFORM_NAME, sizeof(platform_name), &platform_name, NULL);
        platform_status = clGetPlatformInfo (platform_ids[i], CL_PLATFORM_VERSION, sizeof(platform_version), platform_version, NULL);
        if (platform_status == CL_SUCCESS)
        {
          printf ("* PLATFORM (VERSION): \t%s (%s)\n", platform_name, platform_version);
        }
        else
        {
          printf ("Platform Status: Error %i in clGetPlatformInfo call!\n", platform_status);
          is_platforms = false;
        }
      }
    }
  }
 
  return platform_ids; 
}


cl_device_id * devices_summary (cl_platform_id platform_id)
{
  bool is_devices = true;
  char platform_name[1024];

  cl_uint n_devices;
  cl_device_id * device_ids;
  cl_int platforms_status;
  cl_int devices_status;

  printf ("Devices Summary\n");
  devices_status = clGetDeviceIDs (platform_id, CL_DEVICE_TYPE_ALL, 0, NULL, &n_devices);

  if (n_devices == 0)
  {
    printf ("Devices Status: No devices found.\n");
    is_devices = false;
  }
  else if (devices_status != CL_SUCCESS)
  {
    printf ("Devices Status: Error %i in clGetDeviceIDs call!\n", devices_status);
    is_devices = false;
  }
  else
  {
    platforms_status = clGetPlatformInfo (platform_id, 
                                          CL_PLATFORM_NAME, 
                                          sizeof(platform_name),
                                          platform_name,
                                          NULL);
    printf ("* %s: %u devices found.\n", platform_name, n_devices);

    device_ids = (cl_device_id*)malloc(n_devices * sizeof(cl_device_id));
    if (device_ids == NULL)
    {
      printf ("Devices Status: Failed to allocate memory for devices.\n");
      is_devices = false;
    }

    devices_status = clGetDeviceIDs (platform_id,
                                     CL_DEVICE_TYPE_ALL,
                                     n_devices,
                                     device_ids,
                                     &n_devices);
    if (devices_status == CL_SUCCESS)
    {
      for (unsigned int i = 0; i < n_devices; ++i)
      {
        printf ("  DEVICE %d\n", i);
        DEVICE device = get_device_info (device_ids[i]);
      }
    }
    else
    {
      printf ("Devices Status: %i in clGetDeviceIDs call.\n", devices_status);
      is_devices = false;
    }
  }

  return device_ids;
}


DEVICE get_device_info (cl_device_id id)
{
  DEVICE device;
  
  clGetDeviceInfo (id, CL_DEVICE_NAME, sizeof(device.name), &device.name, NULL);
  clGetDeviceInfo (id, CL_DEVICE_VENDOR, sizeof(device.vendor), &device.vendor, NULL);
  clGetDeviceInfo (id, CL_DRIVER_VERSION, sizeof(device.driver_version), &device.driver_version, NULL);
  clGetDeviceInfo (id, CL_DEVICE_TYPE, sizeof(device.cl_type), &device.cl_type, NULL);
  clGetDeviceInfo (id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(device.compute_units), &device.compute_units, NULL);
  clGetDeviceInfo (id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(device.workitem_dims), &device.workitem_dims, NULL);
  clGetDeviceInfo (id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(device.workitem_size), &device.workitem_size, NULL);
  clGetDeviceInfo (id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(device.workgroup_size), &device.workgroup_size, NULL);
  clGetDeviceInfo (id, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(device.clock_frequency), &device.clock_frequency, NULL);
  clGetDeviceInfo (id, CL_DEVICE_ADDRESS_BITS, sizeof(device.addr_bits), &device.addr_bits, NULL);
  clGetDeviceInfo (id, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(device.max_mem_alloc_size), &device.max_mem_alloc_size, NULL);
  clGetDeviceInfo (id, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(device.global_mem_size), &device.global_mem_size, NULL);
  clGetDeviceInfo (id, CL_DEVICE_ERROR_CORRECTION_SUPPORT, sizeof(device.error_correction_support), &device.error_correction_support, NULL);
  clGetDeviceInfo (id, CL_DEVICE_LOCAL_MEM_TYPE, sizeof(device.local_mem_type), &device.local_mem_type, NULL);
  clGetDeviceInfo (id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(device.local_mem_size), &device.local_mem_size, NULL);
  
  printf ("\t CL_DEVICE_NAME: \t%s\n", device.name);
  printf ("\t CL_DEVICE_VENDOR: \t%s\n", device.vendor);
  printf ("\t CL_DRIVER_VERSION: \t%s\n", device.driver_version);
  if (device.cl_type & CL_DEVICE_TYPE_CPU)
    printf ("\t CL_DEVICE_TYPE: \t\%s\n", "CL_DEVICE_TYPE_CPU");
  if (device.cl_type & CL_DEVICE_TYPE_GPU)
    printf ("\t CL_DEVICE_TYPE: \t%s\n", "CL_DEVICE_TYPE_GPU");
  if (device.cl_type & CL_DEVICE_TYPE_ACCELERATOR)
    printf ("\t CL_DEVICE_TYPE: \t%s\n", "CL_DEVICE_TYPE_ACCELERATOR");
  if (device.cl_type & CL_DEVICE_TYPE_DEFAULT)
    printf ("\t CL_DEVICE_TYPE: \t%s\n", "CL_DEVICE_TYPE_DEFAULT");
  printf ("\t CL_DEVICE_MAX_COMPUTE_UNITS: \t%u\n", device.compute_units);
  printf ("\t CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS: \t%u\n", device.workitem_dims);
  printf ("\t CL_DEVICE_MAX_WORK_ITEM_SIZES: \t%u / %u / %d\n", device.workitem_size[0], device.workitem_size[1], device.workitem_size[2]);
  printf ("\t CL_DEVICE_MAX_WORK_GROUP_SIZE: \t%u\n", device.workgroup_size);
  printf ("\t CL_DEVICE_MAX_CLOCK_FREQUENCY: \t%u MHz\n", device.clock_frequency);
  printf ("\t CL_DEVICE_ADDRESS_BITS: \t%u\n", device.addr_bits);
  printf ("\t CL_DEVICE_MAX_MEM_ALLOC_SIZE: \t%u MByte\n", (unsigned int)(device.max_mem_alloc_size/(1024*1024)));
  printf ("\t CL_DEVICE_GLOBAL_MEM_SIZE: \t%u MByte\n", (unsigned int)(device.global_mem_size/(1024*1024)));
  printf ("\t CL_DEVICE_ERROR_CORRECTION_SUPPORT: \t%s\n", device.error_correction_support == CL_TRUE ? "yes" : "no");
  printf ("\t CL_DEVICE_LOCAL_MEM_TYPE: \t%s\n", device.local_mem_type == 1 ? "local" : "global");
  printf ("\t CL_DEVICE_LOCAL_MEM_SIZE: \t%u KByte\n", (unsigned int)(device.local_mem_size/1024));
  

  return device;
   
}


void system_info (void)
{
  char time_desc[255];
  struct tm * ts;

  time_t now = time(NULL);
  ts = localtime (&now);
  strftime (time_desc, 255, " %H:%M:%S, %m/%d/%Y", ts);

  printf ("System Info\n");
  printf ("* Local Time/Date = %s\n", time_desc);
}
