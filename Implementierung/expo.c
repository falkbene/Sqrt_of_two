#include "Expo.h"
#include "Bignum.h"
#include "IntArr.h"
#include <stdlib.h>
#include <stdio.h>
#include "TestNumber.h"
#include "string.h"
#include "stdbool.h"


void matrixmultiply( bignum ** a, bignum ** b){

     bignum matrix1, matrix2, matrix4, matrix3, tmp1, tmp2, tmp3, tmp4; //Initializing the components of 2x2 Matrix
     initbignum(&matrix1,1);
     initbignum(&matrix2,1);
     initbignum(&matrix3,1);
     initbignum(&matrix4,1);
     initbignum(&tmp1,1);
     initbignum(&tmp2,1);
     initbignum(&tmp3,1);
     initbignum(&tmp4,1);

     setbignum(&matrix1, a[0][0].numbers, a[0][0].length);
     mulbignum(&matrix1, &b[0][0]);

     setbignum(&matrix2, a[0][1].numbers, a[0][1].length);
     mulbignum(&matrix2, &b[1][0]);


     setbignum(&matrix3, a[0][0].numbers, a[0][0].length);
     mulbignum(&matrix3, &b[0][1]);

     setbignum(&matrix4, a[0][1].numbers, a[0][1].length);
     mulbignum(&matrix4, &b[1][1]);

     setbignum(&tmp3, a[1][0].numbers, a[1][0].length);
     mulbignum(&tmp3, &b[0][0]);

     setbignum(&tmp4, a[1][1].numbers, a[1][1].length);
     mulbignum(&tmp4, &b[1][0]);

     setbignum(&tmp1, a[1][0].numbers, a[1][0].length);
     mulbignum(&tmp1, &b[0][1]);

     setbignum(&tmp2, a[1][1].numbers, a[1][1].length);
     mulbignum(&tmp2, &b[1][1]);


     addbignum(&matrix1, &matrix2);
     addbignum(&matrix3, &matrix4);
     addbignum(&tmp3, &tmp4);
     addbignum(&tmp1, &tmp2);

     setbignum(&a[0][0], matrix1.numbers, matrix1.length);
     setbignum(&a[0][1], matrix3.numbers, matrix3.length);
     setbignum(&a[1][0], matrix3.numbers, matrix3.length);
     setbignum(&a[1][1], tmp1.numbers, tmp1.length);
    free(tmp1.numbers);
    free(tmp2.numbers);
    free(tmp3.numbers);
    free(tmp4.numbers);
    free(matrix1.numbers);
    free(matrix2.numbers);
    free(matrix3.numbers);
    free(matrix4.numbers);

}

void matrixmultiplyInt( intArr ** a, intArr ** b){

     intArr matrix1, matrix2, matrix4, matrix3, tmp1, tmp2, tmp3, tmp4;
     initInt(&matrix1,1);
     initInt(&matrix2,1);
     initInt(&matrix3,1);
     initInt(&matrix4,1);
     initInt(&tmp1,1);
     initInt(&tmp2,1);
     initInt(&tmp3,1);
     initInt(&tmp4,1);


     setInt(&matrix1, a[0][0].number, a[0][0].length);
     mulInt(&matrix1,&b[0][0]);

     setInt(&matrix2, a[0][1].number, a[0][1].length);
     mulInt(&matrix2,&b[1][0]);


     setInt(&matrix3, a[0][0].number, a[0][0].length);
     mulInt(&matrix3, &b[0][1]);

     setInt(&matrix4, a[0][1].number, a[0][1].length);
     mulInt(&matrix4, &b[1][1]);

     setInt(&tmp3, a[1][0].number, a[1][0].length);
     mulInt(&tmp3, &b[0][0]);

     setInt(&tmp4, a[1][1].number, a[1][1].length);
     mulInt(&tmp4, &b[1][0]);

     setInt(&tmp1, a[1][0].number, a[1][0].length);
     mulInt(&tmp1, &b[0][1] );

     setInt(&tmp2, a[1][1].number, a[1][1].length);
     mulInt(&tmp2, &b[1][1]);


     addInt(&matrix1, &matrix2);
     addInt(&matrix3, &matrix4);
     addInt(&tmp3, &tmp4);
     addInt(&tmp1, &tmp2);

     setInt(&a[0][0], matrix1.number, matrix1.length);
     setInt(&a[0][1], matrix3.number, matrix3.length);
     setInt(&a[1][0], matrix3.number, matrix3.length);
     setInt(&a[1][1], tmp1.number, tmp1.length);
    free(tmp1.number);
    free(tmp2.number);
    free(tmp3.number);
    free(tmp4.number);
    free(matrix1.number);
    free(matrix2.number);
    free(matrix3.number);
    free(matrix4.number);

}

size_t findPower(size_t n) {     //Method to find the lowest x in 2^x > n
    size_t count = 0;
    while (n>0){
        n = n>>1;
        count++;
    }
    return count;
}

void testFunction(char* checkString){       //compare an inputstring to a string of sqrt(2) with 10Million digits
    if(checkString[0]=='1'&&checkString[1] == '.') {
        checkString += (2*sizeof (char ));
    }
    compareString(checkString, SQRT_OF_TWO);
    printf("\n");
}
void expo(size_t exp, size_t gen, int print, bool simd){
//This part created the necessary components to calculate the result, this includes the identiy matrix and the initial matrix {{0,1},{1,2}}

    size_t count = findPower(exp);        //Method to find the lowest n in 2^n > exponent

    bignum **einheitsmatrix = (bignum **) malloc(2 * sizeof (bignum *));
    if(einheitsmatrix == NULL){
        printf("Speicher konnte nicht alloziert werden!");
        exit(1);
    }
    for(int i = 0; i<2;i++){
        einheitsmatrix[i] = (bignum *) malloc(2 * sizeof (bignum));
        if(einheitsmatrix[i] == NULL){
            printf("Speicher konnte nicht alloziert werden!");
            exit(1);
        }
    }
    bignum matrix1, matrix2, matrix4, matrix3, tmp1, tmp2, tmp3, tmp4;

    initbignum(&matrix1, 1);
    initbignum(&matrix2, 1);
    initbignum(&matrix4, 1);
    initbignum(&matrix3, 1);
    initbignum(&tmp1, 1);
    initbignum(&tmp2, 1);
    initbignum(&tmp3, 1);
    initbignum(&tmp4, 1);

    matrix1.numbers[0] = 1;
    matrix2.numbers[0] = 0;
    matrix3.numbers[0] = 0;
    matrix4.numbers[0] = 1;
    tmp1.numbers[0] = 0;
    tmp2.numbers[0] = 1;
    tmp3.numbers[0] = 1;
    tmp4.numbers[0] = 2;

    initbignum(&einheitsmatrix[0][0], 1);
    initbignum(&einheitsmatrix[0][1], 1);
    initbignum(&einheitsmatrix[1][0], 1);
    initbignum(&einheitsmatrix[1][1], 1);
    setbignum(&einheitsmatrix[0][0], matrix1.numbers, matrix1.length);
    setbignum(&einheitsmatrix[0][1], matrix2.numbers, matrix2.length);
    setbignum(&einheitsmatrix[1][0], matrix3.numbers, matrix3.length);
    setbignum(&einheitsmatrix[1][1], matrix4.numbers, matrix4.length);


    bignum **ausgangsmatrix = (bignum **) malloc(2 * sizeof (bignum *));
    if(ausgangsmatrix == NULL){
        printf("Speicher konnte nicht alloziert werden!");
        exit(1);
    }
    for(int i = 0; i<2;i++){
        ausgangsmatrix[i] = (bignum *) malloc(2 * sizeof (bignum));
        if(ausgangsmatrix[i] == NULL){
            printf("Speicher konnte nicht alloziert werden!");
            exit(1);
        }
    }

    initbignum(&ausgangsmatrix[0][0], 1);
    initbignum(&ausgangsmatrix[0][1], 1);
    initbignum(&ausgangsmatrix[1][0], 1);
    initbignum(&ausgangsmatrix[1][1], 1);
    setbignum(&ausgangsmatrix[0][0], tmp1.numbers, tmp1.length);
    setbignum(&ausgangsmatrix[0][1], tmp2.numbers, tmp2.length);
    setbignum(&ausgangsmatrix[1][0], tmp3.numbers, tmp3.length);
    setbignum(&ausgangsmatrix[1][1], tmp4.numbers, tmp4.length);

// End of preparation
//
//Calculation of squared exponentiation
    while(count>0){
        if(exp%2==1){
            matrixmultiply(einheitsmatrix, ausgangsmatrix);     //Multiplying the result with the accumulator in case                                                     of one in the binary representation
        }
        matrixmultiply(ausgangsmatrix,ausgangsmatrix);

        exp = exp>>1;
        count--;
    }
    if(print == 1) {
        char* str = divbignum(&einheitsmatrix[0][1], &einheitsmatrix[1][1], (long long) gen, simd);
        //returns the result as fixed-comma string and decimal
        printf("Result: 1.%s \n", str);
     //   testFunction(str);
        free(str);
    }
    else if(print == 2) {
    //returns the result as hex string
        printf("Result: 1.%s \n", intToHex((divbignum(&einheitsmatrix[0][1], &einheitsmatrix[1][1], (long long ) gen, simd))));

    }
    else if(print == 3 ) {
    //compare the result to the correct string
        char* finalString = divbignum(&einheitsmatrix[0][1], &einheitsmatrix[1][1], (long long) gen, simd);
        printf("Result: 1.%s\n", finalString);
        testFunction(finalString);
        free(finalString);
    }
    free(tmp1.numbers);
    free(tmp2.numbers);
    free(tmp3.numbers);
    free(tmp4.numbers);
    free(matrix1.numbers);
    free(matrix2.numbers);
    free(matrix3.numbers);
    free(matrix4.numbers);
    free(ausgangsmatrix[0][0].numbers);
    free(ausgangsmatrix[0][1].numbers);
    free(ausgangsmatrix[1][0].numbers);
    free(ausgangsmatrix[1][1].numbers);
    free(einheitsmatrix[0][0].numbers);
    free(einheitsmatrix[0][1].numbers);
    free(einheitsmatrix[1][0].numbers);
    free(einheitsmatrix[1][1].numbers);
    for(int i = 0; i<2; i++){
        free(einheitsmatrix[i]);
    }
    free(einheitsmatrix);
    for(int i = 0; i<2; i++){
        free(ausgangsmatrix[i]);
    }
    free(ausgangsmatrix);
}


void expoInt(size_t exp, size_t gen, int print){
//This part created the necessary components to calculate the result, this includes the identiy matrix and the initial matrix {{0,1},{1,2}}

    size_t count = findPower(exp);

    intArr **einheitsmatrix = (intArr **) malloc(2 * sizeof (intArr *));
    if(einheitsmatrix == NULL){
        printf("Speicher konnte nicht alloziert werden!");
        exit(1);
    }
    for(int i = 0; i<2;i++){
        einheitsmatrix[i] = (intArr *) malloc(2 * sizeof (intArr));
        if(einheitsmatrix[i] == NULL){
            printf("Speicher konnte nicht alloziert werden!");
            exit(1);
        }
    }
    intArr matrix1, matrix2, matrix4, matrix3, tmp1, tmp2, tmp3, tmp4;

    initInt(&matrix1, 1);
    initInt(&matrix2, 1);
    initInt(&matrix4, 1);
    initInt(&matrix3, 1);
    initInt(&tmp1,1);
    initInt(&tmp2,1);
    initInt(&tmp3,1);
    initInt(&tmp4,1);

    matrix1.number[0] = 1;
    matrix2.number[0] = 0;
    matrix3.number[0] = 0;
    matrix4.number[0] = 1;
    tmp1.number[0] = 0;
    tmp2.number[0] = 1;
    tmp3.number[0] = 1;
    tmp4.number[0] = 2;

    initInt(&einheitsmatrix[0][0],1);
    initInt(&einheitsmatrix[0][1],1);
    initInt(&einheitsmatrix[1][0],1);
    initInt(&einheitsmatrix[1][1],1);
    setInt(&einheitsmatrix[0][0],matrix1.number,matrix1.length);
    setInt(&einheitsmatrix[0][1],matrix2.number,matrix2.length);
    setInt(&einheitsmatrix[1][0],matrix3.number,matrix3.length);
    setInt(&einheitsmatrix[1][1],matrix4.number,matrix4.length);


    intArr **ausgangsmatrix = (intArr **) malloc(2 * sizeof (intArr *));
    if(ausgangsmatrix == NULL){
        printf("Speicher konnte nicht alloziert werden!");
        exit(1);
    }
    for(int i = 0; i<2;i++){
        ausgangsmatrix[i] = (intArr *) malloc(2 * sizeof (intArr));
        if(ausgangsmatrix[i] == NULL){
            printf("Speicher konnte nicht alloziert werden!");
            exit(1);
        }
    }

    initInt(&ausgangsmatrix[0][0],1);
    initInt(&ausgangsmatrix[0][1],1);
    initInt(&ausgangsmatrix[1][0],1);
    initInt(&ausgangsmatrix[1][1],1);
    setInt(&ausgangsmatrix[0][0],tmp1.number,tmp1.length);
    setInt(&ausgangsmatrix[0][1],tmp2.number,tmp2.length);
    setInt(&ausgangsmatrix[1][0],tmp3.number,tmp3.length);
    setInt(&ausgangsmatrix[1][1],tmp4.number,tmp4.length);

    //End of creating components

    //Calculation of squared Exponentiation
    while(count>0){
        if(exp%2==1){
            matrixmultiplyInt(einheitsmatrix, ausgangsmatrix);
        }
        matrixmultiplyInt(ausgangsmatrix,ausgangsmatrix);

        exp = exp>>1;
        count--;
    }
    if(print == 1) {

        printf("Result: 1.%s \n", (divLongInt(&einheitsmatrix[0][1], &einheitsmatrix[1][1], (long long) gen)));

    }
    else if(print == 2) {

        printf("Result: 1.%s \n", intToHex((divLongInt(&einheitsmatrix[0][1], &einheitsmatrix[1][1], (long long ) gen))));

    }
    else if(print == 3 ) {

        char* finalString = divLongInt(&einheitsmatrix[0][1], &einheitsmatrix[1][1], (long long) gen);
        printf("Result: 1.%s\n", finalString);
        testFunction(finalString);
        free(finalString);
    }
    //
    else{

        divLongInt(&einheitsmatrix[0][1], &einheitsmatrix[1][1], (long long) gen);

    }
    free(tmp1.number);
    free(tmp2.number);
    free(tmp3.number);
    free(tmp4.number);
    free(matrix1.number);
    free(matrix2.number);
    free(matrix3.number);
    free(matrix4.number);
    free(ausgangsmatrix[0][0].number);
    free(ausgangsmatrix[0][1].number);
    free(ausgangsmatrix[1][0].number);
    free(ausgangsmatrix[1][1].number);
    free(einheitsmatrix[0][0].number);
    free(einheitsmatrix[0][1].number);
    free(einheitsmatrix[1][0].number);
    free(einheitsmatrix[1][1].number);

    for(int i = 0; i<2; i++){
        free(einheitsmatrix[i]);
    }
    free(einheitsmatrix);
    for(int i = 0; i<2; i++){
        free(ausgangsmatrix[i]);
    }
    free(ausgangsmatrix);
}