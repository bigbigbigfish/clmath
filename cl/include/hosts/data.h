void generate_int_data_asc (int * A, int len)
{
  for (int i = 0; i < len; ++i)
  {
    A[i] = i;
  }
}


void generate_int_data_desc (int * A, int len)
{
  for (int i = 0; i < len; ++i)
  {
    A[i] = len - i;
  }
}


