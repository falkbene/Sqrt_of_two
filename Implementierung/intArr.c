#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "IntArr.h"
#include "Expo.h"
#include "string.h"

// initializes a new bignum with the given length
void initInt(intArr * num, size_t length) {

    // allocates the space needed and sets the length
    num->number = malloc(length * sizeof(long long));
    if( num->number == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }
    num->length = length;

}

// sets the number array of the given intArr to the value array and sets the length accordingly
void setInt(intArr* num, const int * value, size_t length) {

    // allocates new space if needed and sets the length accordingly
    if (length > num->length) {
        free(num->number);
        num->number = malloc(length * sizeof(long long));
        if(num->number == NULL) {
            printf("Speicher konnte nicht zugewiesen werden!\n");
            exit(1);
        }
    }
    num->length = length;

    // copies the values of the value array in the number array of the intArr
    for (size_t i = 0; i < length; i++) {
        num->number[i] = value[i];

    }
}

// sets a value array as the numbers array of a intArr
void setShort(intArr* num, int * value, size_t length) {

    if(num->number != NULL) {
        free(num->number);
    }
    num->number = value;
    num->length = length;
}

// prints an intArr
void printInt(intArr* num) {

    // prints the most significant array without leading zeros
    printf("%d",num->number[num->length - 1]);

    // prints the other arrays, but also prints zeros if necessary
    for (size_t i = 1; i < num->length; ++i) {
        int x = num->number[num->length - i -1 ];
        if (x < 10) {
            printf("0");
        } if(x < 100) {
            printf("0");
        } if(x < 1000) {
            printf("0");
        }
        printf("%d",x);
    }

}

// removes leading zeros
void removeLeadingZerosInt(intArr *num) {

    long long i;

    // iterates over the array until the current array != 0
    // --> i = first index with array[ index ] != 0
    for (i = num->length; i > 0;) {
        if (num->number[--i] != 0) {
            break;
        }
    }

    // return if there are no leading zeros
    if(i == num->length -1)
        return;

    // allocates new space for the array with size = amount of arrays without leading zeros
    int *arr = malloc((i+1) * sizeof(long long));
    if( arr == NULL) {
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }

    // copies the values without the leading zeros int arr and sets the intArr accordingly
    int x = 0;
    while (i >= 0) {
        arr[i] = num->number[i];
        x++;
        i--;
    }
    setShort(num, arr, x);
}

// adds two intArr and safes the result in the first one, the second one stays unchanged
void addInt(intArr *num1, intArr *num2) {

    if(num1->length >= num2->length) {

        // creats an array with length = length of num1 + 1 --> maximum length of the result
        int * arr = malloc((num1->length+1) * sizeof (long long));
        if( arr == NULL) {
            printf("Speicher konnte nicht zugewiesen werden!\n");
            exit(1);
        }

        int carry = 0;
        int i = 0;

        // adds the two intArr array-wise with a carry and writes the resukt mod 1 billion into the result array
        // carry = 1 if result of the prvious addition was >= 1 billion else 0
        while (i < num2->length) {
            int x = num1->number[i] + num2->number[i] + carry;
            carry = x / 10000;
            arr[i++] = x % 10000;
        }

        // if the length of the first number is greater then the length of the second number the remaining arrays of the first number are added to the result
        while (i < num1->length) {
            int x = num1->number[i]  + carry;
            carry = x / 10000;
            arr[i++] = x % 10000;
        }

        arr[i] = carry;

        // if the carry = 1 then the length need to be increased, otherwise the length of the longer number is enough
        setShort(num1, arr, num1->length + carry);

    } else {
        // works the same as the other method, but num2 ist uses as num1 was used before

        int * arr = malloc((num2->length+1) * sizeof (long long));
        if( arr == NULL){
            printf("Speicher konnte nicht zugewiesen werden!\n");
            exit(1);
        }
        int carry = 0;
        size_t i = 0;
        while (i < num1->length) {
            int x = num1->number[i] + num2->number[i] + carry;
            carry = x / 10000;
            arr[i++] = x % 10000;
        }
        while (i < num2->length) {
            int x = num2->number[i]  + carry;
            carry = x / 10000;
            arr[i++] = x % 10000;
        }
        arr[i] = carry;
        setShort(num1, arr, num2->length + carry);
    }
}

// multiplies two intArr and safes the result in the first one, the second one stays unchanged
void mulInt(intArr * num1, intArr *num2) {

    // creates an array with the right length and sets all Elements to zero
    size_t length = num1->length + num2->length +1;
    int * arr = malloc((length) * sizeof (long long));
    if( arr == NULL) {
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }
    long long i, j;
    for(i = 0; i < length; i++) {
        arr[i] = 0;
    }

    // multiplies the numbers array-wise
    // writes the result mod 1 billion in the result array add the result / 1 billion into to the next array
    for(i = 0; i < num1->length;i++) {
        for(j = 0; j < num2->length;j++) {
            int x = num1->number[i] * num2->number[j];
            arr[i +j +1] += x / 10000;
            arr[i + j] += x % 10000;
        }
    }

    // writes the result int num1 and removes leading zeros
    setShort(num1, arr, length);
    removeLeadingZerosInt(num1);

}

void subInt(intArr * num1, intArr *num2) {
    // creates an array with size = the length of num1 and fills it with zeros
    // subtracts array wise
    long long i = 0;
    while ( i < num2->length / 6) {
        num1->number[i] = num1->number[i] - (num2->number[i]);
        if (num1->number[i] < 0) {
            num1->number[i] += 10000;
            num1->number[i+1] -= 1;
        }
        num1->number[i+1] = num1->number[i+1] - (num2->number[i+1]);
        if (num1->number[i+1] < 0) {
            num1->number[i+1] += 10000;
            num1->number[i+2] -= 1;
        }
        num1->number[i+2] = num1->number[i+2] - (num2->number[i+2]);
        if (num1->number[i+2] < 0) {
            num1->number[i+2] += 10000;
            num1->number[i+3] -= 1;
        }
        num1->number[i+3] = num1->number[i+3] - (num2->number[i+3]);
        if (num1->number[i+3] < 0) {
            num1->number[i+3] += 10000;
            num1->number[i+4] -= 1;
        }
        num1->number[i+4] = num1->number[i+4] - (num2->number[i+4]);
        if (num1->number[i+4] < 0) {
            num1->number[i+4] += 10000;
            num1->number[i+5] -= 1;
        }
        num1->number[i+5] = num1->number[i+5] - (num2->number[i+5]);
        if (num1->number[i+5] < 0) {
            num1->number[i+5] += 10000;
            num1->number[i+6] -= 1;
        }
        i += 6;
    }

    while ( i < num2->length) {
        num1->number[i] = num1->number[i] - (num2->number[i]);
        if (num1->number[i] < 0) {
            num1->number[i] += 10000;
            num1->number[i+1] -= 1;
        }
        i++;
    }

    removeLeadingZerosInt(num1);

}

// compares two intArrs
// return 1 if num1 > num2; -1 if num1 < num2; 0 if num1 = num2
int cmpInt(intArr* num1, intArr* num2) {

    // in our implementation before evey call of cmpInt the leading zeros are removed, so it is not mandatory to do it here again

    // if one number is longer than the other, it is longer
    if(num1->length > num2->length)
        return 1;
    if(num2->length > num1->length)
        return -1;
    // compares all arrays starting with the most significant one
    for(long long i = num1->length-1; i >=0;i--){
        //      printf("I = %d\n",i);
        if(num1->number[i] > num2->number[i]){
            return 1;
        }
        if(num2->number[i] > num1->number[i]) {
            return -1;
        }
    }
    // if num1 is neither larger nor smaller than num2, the numbers have to be equal
    return 0;
}

// divides num1 / num2 up to the given precision
char* divLongInt(intArr * num1, intArr *num2, long long pre){
    // creates a new intArr to shift the comma
    intArr ten;
    initInt(&ten, 1);
    ten.number[0] = 10;
    char * srt = malloc( (1+pre) * sizeof (char ));
    if( srt == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }
    long long x = -1;
    long long i;
    for( i = 0; i <= pre; i++){

        int counter = 0;

        // checks how often num1 fits in num2
        while (cmpInt(num1, num2) > 0){

            subInt(num1, num2);
            counter++;

        }

        // if the numbers are equal the programm terminates
        if(cmpInt(num1, num2) == 0){
            srt[x++] = (counter+1) + '0';
            break;
        }

        // writes the counter in the str Array
        if(x < 0){
            x++;
        }else {
            srt[x++] = counter + '0';
        }

        // shifts the comma to the right
        mulInt(num1, &ten);

    }
    srt[x] = '\0';
    return srt;
}

// print = 1 prints the result, prints the result in hexadecimal, 0 compares the String
void exp1Int(size_t e, size_t g, int print){

    intArr matrix1,matrix2,matrix4,tmp1,tmp2,tmp3,tmp4;

    // initializes the values and set the matrix
    initInt(&matrix1, 1);
    initInt(&matrix2, 1);
    initInt(&matrix4, 1);
    initInt(&tmp1, 1);
    initInt(&tmp2, 1);
    initInt(&tmp3, 1);
    initInt(&tmp4, 1);

    matrix1.number[0] = 0;
    matrix2.number[0] = 1;
    matrix4.number[0] = 2;
    tmp1.number[0] = 0;
    tmp2.number[0] = 0;
    tmp3.number[0] = 0;
    tmp4.number[0] = 0;

    // each iteration squares the matrix
    if(e<4) {
        e = 4;  //Minimum required exponentiation
    }
    for(int i = 0; i < e; i++){

        setInt(&tmp1, matrix1.number, matrix1.length);
        mulInt(&tmp1, &matrix1);
        setInt(&tmp2, matrix2.number, matrix2.length);
        mulInt(&tmp2, &matrix2);
        addInt(&tmp1, &tmp2);

        setInt(&tmp2, matrix2.number, matrix2.length);
        mulInt(&tmp2, &matrix1);
        setInt(&tmp3, matrix2.number, matrix2.length);
        mulInt(&tmp3, &matrix4);
        addInt(&tmp2, &tmp3);

        setInt(&tmp4, tmp2.number, tmp2.length);
        addInt(&tmp4, &tmp2);
        addInt(&tmp4, &tmp1);

        setInt(&matrix1, tmp1.number, tmp1.length);
        setInt(&matrix2, tmp2.number, tmp2.length);
        setInt(&matrix4, tmp4.number, tmp4.length);

    }

    // executes the given print command
    if(print == 1)
        printf("Result: 1.%s\n",divLongInt(&matrix2,&matrix4,(long long) g));
    else if(print == 2) {
        printf("Result: 1.%s\n", intToHex(divLongInt(&matrix2,&matrix4,(long long) g)));
    }
    else if(print == 3){
        char* finalString = divLongInt(&matrix2, &matrix4, (long long) g);
        printf("Result: 1.%s\n", finalString);
        testFunction(finalString);
    }
}
