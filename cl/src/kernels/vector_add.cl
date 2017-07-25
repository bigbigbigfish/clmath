__kernel void vector_add (__global const int * A,
                          __global const int * B,
                          __global int * C)
{
  // get the index of the current element to be processed
  int i = get_global_id (0);

  // do the operation
  C[i] = A[i] + B[i];
}


__kernel void vector_add_float (__global const float * A,
                                __global const float * B,
                                __global float * result)
{
  int gid = get_global_id (0);
  result[gid] = A[gid] + B[gid];
}
