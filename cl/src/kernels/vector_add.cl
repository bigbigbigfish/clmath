__kernel void vector_add (__global float * A,
                          __global float * B,
                          __global float * C,
                          const unsigned int count)
{
  int i = get_global_id (0);
  if (i < count)
    C[i] = A[i] + B[i];
}
