__kernel void matrix_mul (__global float * A,
                          __global float * B,
                          __global float * C,
                          const unsigned int M,
                          const unsigned int N,
                          const unsigned int K)
{
  int x;
  int i = get_global_id (0);
  int j = get_global_id (0);
  float tmp;

  if ((i < M) && (j < N))
  {
    tmp = 0.0f;
    for (x = 0; x < K; ++x)
      tmp += A[i*K+x] * B[x*N+j];
    C[i*N+j] = tmp;
  }
}
