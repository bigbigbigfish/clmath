__kernel void matrix_mul (__global float * A,
                          __global float * B,
                          __global float * C,
                          const unsigned int count)
{
  int k;
  int i = get_global_id (0);
  int j = get_global_id (0);
  float tmp;

  if ((i < N) && (j < N))
  {
    tmp = 0.0f;
    for (k = 0; k < count; ++k)
      tmp += A[i*count+k] * B[k*count+j];
    C[i*count+j] = tmp;
  }
}
