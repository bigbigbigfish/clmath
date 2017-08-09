#define TS (32)
#define WPT (8)
#define RTS (4)


// matrix_mul_vector
#define WIDTH 4
#if WIDTH == 1
  typedef float floatX;
#elif WIDTH == 2
  typedef float2 floatX;
#elif WIDTH == 4
  typedef float4 floatX;
#endif

// matrix_mul_transpose
#define TSM 64                           // the tile-size in dimension M
#define TSN 64                           // the tile-size in dimension N
#define TSK 32                           // the tile-size in dimension K
#define WPTN 8                           // the work-per-thread in dimension N
#define RTSM 8                           // the reduced tile-size in dimension M
#define RTSN (TSN/WPTN)                  // the reduced tile-size in dimension N
#define LPT ((TSK*TSM)/(RTSM*RTSN))      // the loads-per-thread for a tile
#define TRANSPOSEX 8
#define TRANSPOSEY 4

/* --------------------------------------------------------------------------------------------- */

__kernel void matrix_mul_global (const __global float * d_A,
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


/* --------------------------------------------------------------------------------------------- */

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

/* --------------------------------------------------------------------------------------------- */

/*
 * TS = WPT * RTS
 * - TS: number of tiles
 * - WPT: x elements per thread
 * - RTS: number of sub-tiles
 * 
 * For instance:
 * - TS: 32, WPT: 8, RTS: 4
 * - TS: 32, WPT: 16, RTS: 2
 */
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


__kernel void matrix_mul_vector (const __global floatX * d_A,
                                 const __global floatX * d_B,
                                 __global floatX * d_C,
                                 const unsigned int M,
                                 const unsigned int N,
                                 const unsigned int K)
{
  // thread identifiers
  const int local_row = get_local_id (0);
  const int local_col = get_local_id (1);
  const int global_row = (TS/WIDTH) * get_group_id(0) + local_row;
  const int global_col = TS * get_group_id(1) + local_col;

  // local memory to fit a tile of TS*TS elements of A and B
  __local floatX d_A_sub[TS][TS/WIDTH];
  __local floatX d_B_sub[TS][TS/WIDTH];

  // initialize the accumulation registers
  #if WIDTH == 1
    floatX acc = 0.0f;
  #elif WIDTH == 2
    floatX acc = {0.0f, 0.0f};
  #elif WIDTH == 4
    floatX acc = {0.0f, 0.0f, 0.0f, 0.0f};
  #endif

  // loop over all tiles
  const int n_tiles = K/TS;
  for (int t = 0; t < n_tiles; ++t)
  {
    // load one tile of A and B into local memory
    const int tiled_row = (TS/WIDTH)*t + local_row;
    const int tiled_col = TS*t + local_col;
    d_A_sub[local_col][local_row] = d_A[tiled_col*(M/WIDTH) + global_row];
    d_B_sub[local_col][local_row] = d_B[global_col*(K/WIDTH) + tiled_row];

    // synchronize to make sure the tile is loaded
    barrier(CLK_LOCAL_MEM_FENCE);

    // perform the computation for a single tile
    floatX vector_A, vector_B;
    float val_B;
    for (int k = 0; k < TS/WIDTH; ++k)
    {
      vector_B = d_B_sub[local_col][k];
      for (int w = 0; w < WIDTH; ++w)
      {
        vector_A = d_A_sub[WIDTH*k + w][local_row];
        #if WIDTH == 1
          val_B = vector_B;
          acc += vector_A * vector_B;
        #elif WIDTH == 2
          switch (w)
          {
            case 0: val_B = vector_B.x; break;
            case 1: val_B = vector_B.y; break;
          }
          acc.x += vector_A.x + val_B;
          acc.y += vector_A.y * val_B;
        #elif WIDTH == 4
          switch (w)
          {
            case 0: val_B = vector_B.x; break;
            case 1: val_B = vector_B.y; break;
            case 2: val_B = vector_B.z; break;
            case 3: val_B = vector_B.w; break;
          }
          acc.x += vector_A.x * val_B;
          acc.y += vector_A.y * val_B;
          acc.z += vector_A.z * val_B;
          acc.w += vector_A.w * val_B;
        #endif
      }
    }

    // synchronize before loading the next tile
    barrier(CLK_LOCAL_MEM_FENCE);
  }

  // store the final results in C
  d_C[global_col*(M/WIDTH) + global_row] = acc;
}

/* --------------------------------------------------------------------------------------------- */


/*
 * Simple transpose kernel for a P * Q matrix
 */
__kernel void transpose (const __global float * input,
                         __global float * output,
                         const int P,
                         const int Q)
{
  // thread identifiers
  const int tx = get_local_id (0);
  const int ty = get_local_id (1);
  const int ID0 = get_group_id(0) * TRANSPOSEX + tx;  // 0..P
  const int ID1 = get_group_id(1) * TRANSPOSEY + ty;  // 0..Q
  
  // set-up the local memory for shuffling
  __local float buffer[TRANSPOSEX][TRANSPOSEY];

  // swap the x and y coordinates to perform the rotation (coalesed)
  if (ID0 < P && ID1 < Q)
  {
    buffer[ty][tx] = input[ID1*P + ID0];
  }

  // synchronise all threads
  barrier(CLK_LOCAL_MEM_FENCE);

  // we don't have to swap the x and y thread indices here,
  // because that's already done in the local memory
  const int newID0 = get_group_id(1) * TRANSPOSEY + tx;
  const int newID1 = get_group_id(0) * TRANSPOSEX + ty;

  // store the transposed result (coalesced)
  if (newID0 < Q && newID1 < P)
  {
    output[newID1*Q + newID0] = buffer[tx][ty];
  }
}


/*
 * Pre-transpose the input matrix B and use rectangular tiles
 */
__kernel void matrix_mul_transpose (const __global float * d_A,
                                    const __global float * d_B,
                                    __global float * d_C,
                                    const unsigned int M,
                                    const unsigned int N,
                                    const unsigned int K)
{
  // thread identifiers
  const int local_row = get_local_id (0);
  const int local_col = get_local_id (1);
  const int global_row = TSM * get_group_id (0) + local_row;  // 0..M
  const int global_col = TSM * get_group_id (1) + local_col;  // 0..N

  // local memory to fit a tile of A and B
  __local float d_A_sub[TSK][TSM];
  __local float d_B_sub[TSN][TSK];

  // initialize the accumulation registers
  float acc[WPTN];
  for (int w = 0; w < WPTN; ++w)
  {
    acc[w] = 0.0f;
  }

  // loop over all tiles
  int n_tiles = K/TSK;
  for (int t = 0; t < n_tiles; ++t)
  {
    // load one tile of A and B into local memory
    for (int l = 0; l < LPT; ++l)
    {
      int tiled_index = TSK * t + local_col + l * RTSN;
      int index_A = tiled_index * M + TSM * get_group_id (0) + local_row;
      int index_B = tiled_index * N + TSN * get_group_id (1) + local_row;
      d_A_sub[local_col + l * RTSN][local_row] = d_A[index_A];
      d_B_sub[local_row][local_col + l * RTSN] = d_B[index_B];
    }

    // synchronize to make sure the tile is loaded
    barrier(CLK_LOCAL_MEM_FENCE);

    // perform the computation for a single tile
    for (int k = 0; k < TSK; ++k)
    {
      for (int w = 0; w < WPTN; ++w)
      {
        acc[w] += d_A_sub[k][local_row] * d_B_sub[local_col + w * RTSN][k];
      }
    }

    // synchronize before loading the next tile
    barrier(CLK_LOCAL_MEM_FENCE);
  }

  // store the final results in C
  for (int w = 0; w < WPTN; ++w)
  {
    d_C[(global_col + w * RTSN) * M + global_row] = acc[w];
  }
}

