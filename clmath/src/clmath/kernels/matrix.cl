__kernel void matrix_mul_col (__global float * A,
                              __global float * B,
                              __global float * C,
                              const unsigned int M,
                              const unsigned int N,
                              const unsigned int K)
{
  int k;
  int i = get_global_id (0);
  int j = get_global_id (0);
  float tmp;

  if ((i < M) && (j < N))
  {
    tmp = 0.0f;
    for (k = 0; k < K; ++k)
      tmp += A[i*K+k] * B[k*N+j];
    C[i*N+j] = tmp;
  }
}


__kernel void matrix_mul_row (__global float * A,
                              __global float * B,
                              __global float * C,
                              const unsigned int M,
                              const unsigned int N,
                              const unsigned int K)
{
  int k, j;
  int i = get_global_id (0);
  float tmp;

  if (i < M)
  {
    for (j = 0; j < N; ++j)
    {
      tmp = 0.0f;
      for (k = 0; k < K; ++k)
        tmp += A[i*K+k] * B[k*N+j];
      C[i*N+j] = tmp;
    }
  }
}


__kernel void matrix_mul_private (__global float * A,
                                  __global float * B,
                                  __global float * C,
                                  const unsigned int M,
                                  const unsigned int N,
                                  const unsigned int K)
{
  int k, j;
  int i = get_global_id (0);
  float Awrk[1024];
  float tmp;

  if (i < N)
  {
    for (k = 0; k < K; ++k)
      Awrk[k] = A[i*K+k];

    for (j = 0; j < N; j++)
      tmp = 0.0f;
      for (k = 0; k < N; ++k)
        tmp += Awrk[k] * B[k*N+j];
      C[i*N+j] = tmp;
  }
}
