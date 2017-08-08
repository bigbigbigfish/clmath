__kernel void matrix_mul (const __global float * d_A,
                          const __global float * d_B,
                          __global float * d_C,
                          const unsigned int M,
                          const unsigned int N,
                          const unsigned int K)
{
  // thread identifiers
  const int global_row = get_global_id (0);
  const int global_col = get_global_id (1);

  // compute a single element (loop over K)
  float acc = 0.0f;
  for (int k = 0; k < K; ++k)
    acc += d_A[k*M + global_row] * d_B[global_col*K + k];

  d_C[global_col*M + global_row] = acc;
}
