#define TS (32)
#define WPT (8)
#define RTS (4)


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


__kernel void matrix_mul_shared (const __global float * d_A,
                                 const __global float * d_B,
                                 __global float * d_C,
                                 const unsigned int M,
                                 const unsigned int N,
                                 const unsigned int K)
{
  // thread identifiers
  const int local_row = get_local_id (0);  // local row ID (max: TS)
  const int local_col = get_local_id (1);  // local col ID (max: TS)
  const int global_row = TS * get_group_id (0) + local_row;
  const int global_col = TS * get_group_id (1) + local_col;

  // local memory to fit a tile of TS*TS elements of A and B
  __local float d_A_sub[TS][TS];
  __local float d_B_sub[TS][TS];

  // initialize the accumulation register
  float acc = 0.0f;

  // loop over all tiles
  const int n_tiles = K/TS;
  for (int t = 0; t < n_tiles; ++t)
  {
    // load one tile of A and B into local memory
    const int tiled_row = TS*t + local_row;
    const int tiled_col = TS*t + local_col;
    d_A_sub[local_col][local_row] = d_A[tiled_col*M + global_row];
    d_B_sub[local_col][local_row] = d_B[global_col*K + tiled_row];

    // synchronise to make sure the tile is loaded
    barrier(CLK_LOCAL_MEM_FENCE);

    // perform the computation for a single tile
    for (int k = 0; k < TS; ++k)
    {
      acc += d_A_sub[k][local_row] * d_B_sub[local_col][k];
    }

    // synchronise before loading the next tile
    barrier(CLK_LOCAL_MEM_FENCE);
  }

  // store the final result in C
  d_C[global_col*M + global_row] = acc;
}


__kernel void matrix_mul_register (const __global float * d_A,
                                   const __global float * d_B,
                                   __global float * d_C,
                                   const unsigned int M,
                                   const unsigned int N,
                                   const unsigned int K)
{
  // thread identifiers
  const int local_row = get_local_id (0);
  const int local_col = get_local_id (1);
  const int global_row = TS * get_group_id (0) + local_row;
  const int global_col = TS * get_group_id (1) + local_col;

  // local memory to fit a tile of TS*TS elements of A and B
  __local float d_A_sub[TS][TS];
  __local float d_B_sub[TS][TS];

  // initialize the accumulcation registers
  float acc[WPT];
  for (int w = 0; w < WPT; ++w)
  {
    acc[w] = 0.0f;
  }

  // loop over all tiles
  const int n_tiles = K/TS;
  for (int t = 0; t < n_tiles; ++t)
  {
    // load one tile of A and B into local memory
    for (int w = 0; w < WPT; ++w)
    {
      const int tiled_row = TS*t + local_row;
      const int tiled_col = TS*t + local_col;
      d_A_sub[local_col + w*RTS][local_row] = d_A[(tiled_col + w*RTS)*M + global_row];
      d_B_sub[local_col + w*RTS][local_row] = d_B[(global_col + w*RTS)*K + tiled_row];
    }

    // synchronize to make sure the tile is loaded
    barrier(CLK_LOCAL_MEM_FENCE);

    // perform the computation for a single tile
    for (int k = 0; k < TS; ++k)
    {
      for (int w = 0; w < WPT; ++w)
      {
        acc[w] += d_A_sub[k][local_row] * d_B_sub[local_col + w*RTS][k];
      }
    }

    // synchronize before loading the next tile
    barrier(CLK_LOCAL_MEM_FENCE);
  }

  // store the final results in C
  for (int w = 0; w < WPT; ++w)
  {
    d_C[(global_col + w*RTS)*M + global_row] = acc[w];
  }
}

