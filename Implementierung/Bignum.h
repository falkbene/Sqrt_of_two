#ifndef UNTITLED1_NEWLONG_H
#define UNTITLED1_NEWLONG_H
#include "stdlib.h"
#include "stdbool.h"

typedef struct {
    long long * numbers;
    size_t length;
    // convention for storing bignums
    // the number 5 123456789 987654321 is stored in 3 Arrays
    // [2] = 5; [1] = 123456789 ; [0] = 987654321
    // -->  [5] [123456789] [987654321]
    // length = amount of arrays
    // --> numbers between 0 und 100.000.000 have lenght = 1
    // -> real length of the number = (length - 1) * 8 + amount of digits in num [num.length -1]
} bignum;

void initbignum(bignum* num, size_t length);
void setbignum(bignum* num, const long long * value, size_t length);
void printbignum(bignum* num);
void addbignum(bignum *num1, bignum *num2);
void mulbignum(bignum * num1, bignum *num2);
void subbignum(bignum * num1, bignum *num2);
void mul10bignum(bignum *num);
char* divbignum(bignum * num1, bignum *num2, long long precision, bool simd);
int compareString(char *a, const char *b);
void squarebingum(size_t e, size_t g, int print);
char * intToHex(char * intStr);
#endif //UNTITLED1_NEWLONG_H
