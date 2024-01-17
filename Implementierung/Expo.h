#ifndef TEAM105_EXPO_H
#define TEAM105_EXPO_H
#include "Bignum.h"
#include "stdbool.h"
void matrixmultiply(bignum ** a, bignum ** b);
void expoInt(size_t exp, size_t gen, int print);
void  expo(size_t exp, size_t gen, int print,bool simd);
void testFunction(char* string);
#endif //TEAM105_EXPO_H