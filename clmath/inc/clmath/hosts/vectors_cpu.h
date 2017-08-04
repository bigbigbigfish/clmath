#ifndef VECTORS_H
#define VECTORS_H

void vector_add_cpu (float * h_A,
                     float * h_B,
                     float * h_C,
                     float * h_D,
                     float * h_E,
                     float * h_F,
                     float * h_G,
                     const unsigned int count);

void vector_add_plus_cpu (float * h_A,
                          float * h_B,
                          float * h_C,
                          float * h_D,
                          const unsigned int count);


#endif
