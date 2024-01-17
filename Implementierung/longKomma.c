#include <stdio.h>
#include <stdlib.h>
#include "LongKomma.h"


// initializes a new LongKomma with the given length
void initLongKomma(newLongKomma* num, size_t length) {
    num->number = malloc(length * sizeof(long long));
    num->length = length;
    num->komma = length;
}

// sets the numbers array of the given LongKomma to the value array and sets the length and comma accordingly
void setLongKomma(newLongKomma* num, const long long * value, size_t length, size_t komma) {
    if (length > num->length) {
        printLongKomma(num);
        num->number = realloc(num->number, length * sizeof(long long));
    }
    num->length = length;
    num->komma = komma;
    for (size_t i = 0; i < length; i++) {
        num->number[i] = value[i];
    }
}

// prints a LongKomma
void printLongKomma(newLongKomma* num) {
    size_t komma = num->komma;

    // prints the comma if necessary
    if(komma == 0) {
        komma--;
        printf("0,");
    }

    // print all zeros and the comma
    for (size_t i = 0; i <num->length ; ++i) {
        long long x = num->number[num->length - i -1 ];
        if(komma == 0)
            printf(",");
        if(x < 10){
            printf("0");
        }if(x < 100){
            printf("0");
        }if(x < 1000){
            printf("0");
        }if(x < 10000){
            printf("0");
        }if(x < 100000){
            printf("0");
        }if(x < 1000000){
            printf("0");
        }if(x < 10000000){
            printf("0");
        }
        if(x < 10000000){
            printf("0");
        }
        printf("%lld",x);
        komma--;
    }
}

// removes the leading and following zeros
void removeZeros(newLongKomma *num) {
    size_t x = num->length -1;

    // removes the leading zeros in front of the comma
    while (num->komma > 0){
        if(num->number[x--] == 0){
            num->komma--;
            num->length--;
        }else
            break;
    }
    x = 0;

    // removes the following zeros
    while (num->length > 1){
        if(num->number[x++] != 0)
            break;
    }

    // creates a new array and sets it as the value of num
    size_t y  = num->length -x +1;
    long long *arr = malloc(y * sizeof(long long));
    x--;
    for (long i = 0; i < y; i++) {
        arr[i] = num->number[x++];
    }

    setLongKomma(num,arr,y,num->komma);
    free(arr);

}

// adds two LongKommas and safes the result in the first one, the second one stays unchanged
void addLongKomma(newLongKomma *num1, newLongKomma *num2) {
    size_t nachKomma1 = num1->length - num1->komma;
    size_t nachKomma2 = num2->length - num2->komma;

    // calculates the longest amount of numbers before and after the comma
    size_t vorKomma = (num1->komma > num2->komma) ? num1->komma +1: num2->komma+1;
    size_t nachKomma = (nachKomma1 > nachKomma2) ? nachKomma1 : nachKomma2;

    // initialize the longest possible result array
    long long * arr = malloc((nachKomma + vorKomma + 1) * sizeof (long long));
    long counter = 0;
    long counter1 = 0;
    long counter2 = 0;

    // if one has more numbers after the comma than the other number it just adds them to the result
    while (nachKomma1 > nachKomma2){
        arr[counter++] = num1->number[counter1++];
        nachKomma1--;
    }
    while (nachKomma2 > nachKomma1){
        arr[counter++] = num2->number[counter2++];
        nachKomma2--;
    }
    long carry = 0;

    // adds the numbers after the comma together
    while (nachKomma1 > 0){
        long long x = num1->number[counter1++] + num2->number[counter2++] + carry;
        carry = x / 1000000000;
        arr[counter++] = x % 1000000000;
        nachKomma1--;
    }

    // adds the numbers before the comma together
    while (num1->length > counter1 && num2->length > counter2){
        long long x = num1->number[counter1++] + num2->number[counter2++] + carry;
        carry = x / 1000000000;
        arr[counter++] = x % 1000000000;
    }

    // if one has more numbers before the comma than the other number it just adds them to the result
    while (num1->length > counter1){
        long long x = num1->number[counter1++] +  carry;
        carry = x / 1000000000;
        arr[counter++] = x % 1000000000;
    }
    while (num2->length > counter2){
        long long x = num2->number[counter2++] +  carry;
        carry = x / 1000000000;
        arr[counter++] = x % 1000000000;
    }

    // sets the result in num1 and removes Zeros
    arr[counter] = carry;
    setLongKomma(num1,arr,vorKomma + nachKomma,vorKomma);
    removeZeros(num1);
    free(arr);

    }

// multiplies two intArrs and safes the result in the first one, the second one stays unchanged
void mulLongKomma(newLongKomma * num1, newLongKomma *num2) {
    size_t nachKomma1 = num1->length - num1->komma;
    size_t nachKomma2 = num2->length - num2->komma;

    // creates the array with the right length and fills it with zeros
    long long length = num1->length + num2->length +1;
    long long * arr = malloc((length) * sizeof (long long));
    size_t i;
    for(i = 0; i < length;i++){
        arr[i] = 0;
    }

    size_t j;

    // multiplies array wise
    for(i = 0; i < num1->length;i++){
        for(j = 0; j < num2->length;j++){
            long long x = num1->number[i] * num2->number[j];
            arr[i +j +1] += x / 1000000000;
            arr[i + j] += x % 1000000000;
        }
    }

    // sets the result in num1
    setLongKomma(num1,arr,length, (length -(nachKomma2 + nachKomma1 )));
    free(arr);
    removeZeros(num1);

}

// compares two bignums
// return 1 if num1 > num2; -1 if num1 < num2; 0 if num1 = num2
int cmpKomma(newLongKomma* num1,newLongKomma* num2) {

    // if one number is longer than the other it is bigger
    if(num1->length > num2->length)
        return 1;
    if(num2->length > num1->length)
        return -1;

    // compares all arrays individually
    for(long long i = num1->length-1; i >=0;i--){

        if(num1->number[i] > num2->number[i]){
            return 1;
        }

        if(num2->number[i] > num1->number[i]) {
            return -1;
        }
    }

    // if neither number is bigger, the number have to be equal
    return 0;
}

// subtracts num1 - num2; works only if num >= num2
void subLongKomma(newLongKomma * num1, newLongKomma *num2) {

    long nachKomma1 = num1->length - num1->komma;
    long nachKomma2 = num2->length - num2->komma;

    // calculates the longest amount of numbers before and after the comma
    long vorKomma = (num1->komma > num2->komma) ? num1->komma : num2->komma;
    long nachKomma = (nachKomma1 > nachKomma2) ? nachKomma1 : nachKomma2;


    // initializes the longest possible result and fills it with zeros
    long long * arr = malloc((nachKomma + vorKomma + 1) * sizeof (long long));
    long counter = 0;
    long counter1 = 0;
    long counter2 = 0;

    for(size_t i = 0; i < (nachKomma + vorKomma + 1);i++)
        arr[i] = 0;

    // subtracts the numbers array wise and if the result is negative, takes one from a higher array
    // and add it as one billion to the current array
    while (nachKomma1 > nachKomma2){
        arr[counter++] = num1->number[counter1++];
        nachKomma1--;
    }
    while (nachKomma2 > nachKomma1){
        long diff =  - (num2->number[counter2++]);
        arr[counter] += diff % 1000000000;
        if (arr[counter] < 0) {
            arr[counter] += 1000000000;
            arr[counter +1] -= 1;
        }
        counter++;
        nachKomma2--;
    }
    while (num2->length > counter2) {
        long diff = num1->number[counter1++] - (num2->number[counter2++]);
        arr[counter] += diff % 1000000000;
        if (arr[counter] < 0) {
            arr[counter] += 1000000000;
            arr[counter + 1] -= 1;
        }
        counter++;
    }
    while (num1->length > counter1){
        long diff = num1->number[counter1++];
        arr[counter] += diff % 1000000000;
        if (arr[counter] < 0) {
            arr[counter] += 1000000000;
            arr[counter + 1] -= 1;
        }
        counter++;
    }

    // sets the result as num1 and removes zeros
    setLongKomma(num1,arr,vorKomma + nachKomma,vorKomma);
    free(arr);
    removeZeros(num1);

}
