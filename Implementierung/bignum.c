#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "Bignum.h"
#include "Expo.h"
#include <immintrin.h>



// initializes a new bignum with the given length
void initbignum(bignum* num, size_t length) {

    // allocates the space needed and sets the length
    num->numbers = malloc(length * sizeof(long long));
    if(num->numbers == NULL) {
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }
    num->length = length;

}

// sets the numbers array of the given bignum to the value array and sets the length accordingly
void setbignum(bignum* num, const long long * value, size_t length) {

    // allocates new space if needed and sets the length accordingly
    if (length > num->length) {
        if(num->numbers != NULL) {
            free(num->numbers);
        }
        num->numbers = malloc(length * sizeof(long long));

        // checks if malloc worked
        if(num->numbers == NULL) {
            printf("Speicher konnte nicht zugewiesen werden!\n");
            exit(1);
        }
    }
    num->length = length;

    // copies the values of the value array in the numbers array of the bignum
    for (size_t i = 0; i < length; i++) {
        num->numbers[i] = value[i];

    }
}

// sets an value array as the numbers array of a bignum
void set(bignum* num, long long * value, size_t length) {

    if(num->numbers != NULL) {
        free(num->numbers);
    }
        num->numbers = value;
        num->length = length;
}

// prints a bignum
void printbignum(bignum* num) {

    // prints the most significant array without leading zeros
    printf("%lld",num->numbers[num->length - 1]);

    // prints the other arrays, but also prints zeros if necessary
    for (size_t i = 1; i < num->length; ++i) {
        long long x = num->numbers[num->length - i - 1];
        if (x < 10){
            printf("0");
        } if (x < 100){
            printf("0");
        } if (x < 1000) {
            printf("0");
        } if (x < 10000) {
            printf("0");
        } if (x < 100000) {
            printf("0");
        } if (x < 1000000) {
            printf("0");
        } if (x < 10000000) {
            printf("0");
        } if (x < 10000000) {
            printf("0");
        }
        printf("%lld",x);
    }
}

// removes leading eros
void removeLeadingZeros(bignum *num) {

    long long i;

    // iterates over the array until the current array != 0
    // --> i = first index with array[ index ] != 0
    for (i = (long long) num->length; i > 0;) {
        if (num->numbers[--i] != 0)
            break;
    }

    // return if there are no leading zeros
    if(i == num->length -1)
        return;

    // allocates new space for the array with size = amount of arrays without leading zeros
    long long *arr = malloc((i+1) * sizeof(long long));

    if( arr == NULL) {
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }

    // copies the values without the leading zeros int arr and sets the bignum accordingly
    int x = 0;
    for (; i >= 0; i--) {
        arr[i] = num->numbers[i];
        x++;
    }
    set(num,arr,x);
}

// subtreacts two bignums using SIMD
void subbignumSIMD(bignum * num1, bignum *num2) {
    size_t i = 0;
    size_t length = num2->length;
    length = length - (length % 4);

    // subtracts four long arrays at the same time and writes it back in num1
    for (; i < length; i += 4) {
        __m256i num1_SIMD = _mm256_loadu_si256((__m256i*)&num1->numbers[i]);
        __m256i num2_SIMD = _mm256_loadu_si256((__m256i*)&num2->numbers[i]);
        _mm256_storeu_si256((__m256i*)&num1->numbers[i], _mm256_sub_epi64(num1_SIMD, num2_SIMD));
    }

    for (; i < num2->length; ++i) {
        num1->numbers[i] = num1->numbers[i] - num2->numbers[i];
    }
    i = 0;
    length = num1->length;

    // subtracts the remaining numbers
    while(i < length){
        if(num1->numbers[i] < 0) {
            num1->numbers[i] += 1000000000;
            num1->numbers[i + 1] -= 1;
        }
        i++;
    }
    removeLeadingZeros(num1);
}

// adds two bignums and safes the result in the first one, the second one stays unchanged
void addbignum(bignum *num1, bignum *num2) {

    if(num1->length >= num2->length){

        // creats an array with length = length of num1 + 1 --> maximum length of the result
        long long * arr = malloc((num1->length+1) * sizeof (long long));
        if( arr == NULL){
            printf("Speicher konnte nicht zugewiesen werden!\n");
            exit(1);
        }

        long long carry = 0;
        long long i = 0;

        // adds the two bignums array-wise with a carry and writes the resukt mod 1 billion into the result array
        // carry = 1 if result of the prvious addition was >= 1 billion else 0
        while (i < num2->length){
            long long x = num1->numbers[i] + num2->numbers[i] + carry;
            carry = x / 1000000000;
            arr[i++] = x % 1000000000;
        }

        // if the length of the first numbers is greater then the length of the second numbers the remaining arrays of the first numbers are added to the result
        while (i < num1->length){
            long long x = num1->numbers[i] + carry;
            carry = x / 1000000000;
            arr[i++] = x % 1000000000;
        }

        arr[i] = carry;

        // if the carry = 1 then the length need to be increased, otherwise the length of the longer numbers is enough
        set(num1,arr,num1->length+carry);

    }else {
        // works the same as the other method, but num2 ist uses as num1 was used before

        long long * arr = malloc((num2->length+1) * sizeof (long long));
        if( arr == NULL){
            printf("Speicher konnte nicht zugewiesen werden!\n");
            exit(1);
        }
        long long carry = 0;
        size_t i = 0;
        while (i < num1->length){
            long long x = num1->numbers[i] + num2->numbers[i] + carry;
            carry = x / 1000000000;
            arr[i++] = x % 1000000000;
        }
        while (i < num2->length){
            long long x = num2->numbers[i] + carry;
            carry = x / 1000000000;
            arr[i++] = x % 1000000000;
        }
        arr[i] = carry;
        set(num1,arr,num2->length+carry);
    }
}

// multiplies two bignums and safes the result in the first one, the second one stays unchanged
void mulbignum(bignum * num1, bignum *num2) {

    // creates an array with the right length and sets all Elements to zero
    unsigned long long length = num1->length + num2->length + 1;
    long long * arr = malloc((length) * sizeof (long long));
    if( arr == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }
    long long i, j;
    for(i = 0; i < length; i++) {
        arr[i] = 0;
    }

    // multiplies the numbers array-wise
    // writes the result mod 1 billion in the reult array add the result / 1 billion into to the next array
    for(i = 0; i < num1->length;i++) {
        for(j = 0; j < num2->length;j++) {
            long long x = num1->numbers[i] * num2->numbers[j];
            arr[i +j +1] += x / 1000000000;
            arr[i + j] += x % 1000000000;
        }
    }

   // writes the result int num1 and removes leading zeros
    set(num1,arr,length);
    removeLeadingZeros(num1);

}

// compares two bignums
// return 1 if num1 > num2; -1 if num1 < num2; 0 if num1 = num2

int cmp(bignum* num1,bignum* num2) {
    // in our implementation before evey call of cmpInt the leading zeros are removed, so it is not mandatory to do it here again

    // if one numbers is longer than the other, it is longer
    if(num1->length > num2->length) {
        return 1;
    }
    if(num2->length > num1->length) {
        return -1;
    }

    // compares all arays starting with the most significant one
    for(long long i = (long long) num1->length-1; i >=0;i--){
        if(num1->numbers[i] > num2->numbers[i]){
            return 1;
        }
        if(num2->numbers[i] > num1->numbers[i]) {
            return -1;
        }
    }

    // if num1 is neither larger nor smaller than num2, the numbers have to be equal
    return 0;
}

// subtracts num1 - num2; works only if num >= num2
void subbignum(bignum * num1, bignum *num2){
    long long i = 0;

    // loop unrolling
    while ( i < (num2->length / 6)) {

        // substracts the number from the given Index and writes them in num1
        // if (num1 < 0) 1 billion is added and num1[index + 1] decremented by 1
        num1->numbers[i] = num1->numbers[i] - (num2->numbers[i]);
        if (num1->numbers[i] < 0) {
            num1->numbers[i] += 1000000000;
            num1->numbers[i + 1] -= 1;
        }

        num1->numbers[i + 1] = num1->numbers[i + 1] - (num2->numbers[i + 1]);
        if (num1->numbers[i + 1] < 0) {
            num1->numbers[i + 1] += 1000000000;
            num1->numbers[i + 2] -= 1;
        }

        num1->numbers[i + 2] = num1->numbers[i + 2] - (num2->numbers[i + 2]);
        if (num1->numbers[i + 2] < 0) {
            num1->numbers[i + 2] += 1000000000;
            num1->numbers[i + 3] -= 1;
        }

        num1->numbers[i + 3] = num1->numbers[i + 3] - (num2->numbers[i + 3]);
        if (num1->numbers[i + 3] < 0) {
            num1->numbers[i + 3] += 1000000000;
            num1->numbers[i + 4] -= 1;
        }

        num1->numbers[i + 4] = num1->numbers[i + 4] - (num2->numbers[i + 4]);
        if (num1->numbers[i + 4] < 0) {
            num1->numbers[i + 4] += 1000000000;
            num1->numbers[i + 5] -= 1;
        }

        num1->numbers[i + 5] = num1->numbers[i + 5] - (num2->numbers[i + 5]);
        if (num1->numbers[i + 5] < 0) {
            num1->numbers[i + 5] += 1000000000;
            num1->numbers[i + 6] -= 1;
        }
        i += 6;
    }

    // the remaining length which was not divisible by 6 is calculated here   
    while (i < num2->length) {
        num1->numbers[i] = num1->numbers[i] - (num2->numbers[i]);
        if (num1->numbers[i] < 0) {
            num1->numbers[i] += 1000000000;
            num1->numbers[i + 1] -= 1;
        }
        i++;
    }

    removeLeadingZeros(num1);

}

// a methode to shift the comma to the right
void mul10bignum(bignum *num) {

    // creates the array and fills it with zeros
    long long length = num->length + 1;
    long long *arr = malloc((length) * sizeof(long long));
    if (arr == NULL) {
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }
    size_t i;
    for (i = 0; i < length; i++) {
        arr[i] = 0;
    }

    // multiplies the array with 10 and writes the carry into the next array
    for (i = 0; i < length - 1; i++) {
        long long x = 10 * num->numbers[i];
        arr[i] += x % 1000000000;
        arr[i + 1] = x / 1000000000;
    }

    // sets the numbers array in num1 as the result array from this function
    if (arr[i] == 0) {
        set(num, arr, length - 1);
    } else {
    set(num, arr, length);
    }
}

// divides num1 / num2 up to the given precision
char* divbignum(bignum * num1, bignum *num2, long long precision, bool simd) {
    // creates a new bignum to shift the comma
    bignum ten;
    initbignum(&ten, 1);
    ten.numbers[0] = 10;
    char * string = malloc((1 + precision) * sizeof (char ));
    if(string == NULL){
        printf("Speicher konnte nicht zugewiesen werden!\n");
        exit(1);
    }

    long long x = -1;
    long long i;
    if(simd) {
        for (i = 0; i <= precision; i++) {

            int8_t counter = 0;

            // checks how often num1 fits in num2
            while (cmp(num1, num2) > 0) {
                subbignumSIMD(num1, num2);
                counter++;
            }

            // if the numbers are equal the programm terminates
            if (cmp(num1, num2) == 0) {
                string[x++] = (counter + 1) + '0';
                break;
            }

            // writes the counter in the str Array
            if (x < 0) {
                x++;
            } else {
                string[x++] = counter + '0';
            }

            // shifts the comma to the right
            mulbignum(num1, &ten);
        }
    }
    else{
        for (i = 0; i <= precision; i++) {

            int8_t counter = 0;

            // checks how often num1 fits in num2
            while (cmp(num1, num2) > 0) {
                subbignum(num1, num2);
                counter++;
            }

            // if the numbers are equal the programm terminates
            if (cmp(num1, num2) == 0) {
                string[x++] = (counter + 1) + '0';
                break;
            }

            // writes the counter in the str Array
            if (x < 0) {
                x++;
            } else {
                string[x++] = counter + '0';
            }

            // shifts the comma to the right
            mulbignum(num1, &ten);
        }
    }
    //
    string[x] = '\0';
    free(ten.numbers);

    return string;
}

// compares two Strings
int compareString(char *a, const char *b){
    unsigned long long l = strlen(a);
    int i;

    // compares the two string char by char
    for( i = 0 ; i < l;i++){
        if(a[i] != b[i]){
            printf(" \n Es sind nur %d von %llu Nachkommastellen genau", i, l);
            return i;
        }
    }

    printf("\nAlle %d Nachkommastellen sind genau", i);
}

// divide a string that represents a very long number by 16
char* div16(char* str) {
    int i = 0;
    int counter = 0;
    unsigned long long length = strlen(str);
    if(length == 0)
        return "0";

    // create the result array
    char* divStr = malloc(length *2 * sizeof (char));
    if(divStr == NULL){
        printf("Speicher konnte nicht alloziert werden");
        exit(1);
    }
    long tmp = str[i++] - '0';

    // adds numbers from the string until the number is bigger than 16, then divides it by 16
    while (str[i] != '\0') {
        tmp = tmp * 10 + (str[i++] - '0');

        while (tmp < 16 && str[i] != '\0') {
            divStr[counter++] = '0';
            tmp = tmp * 10 + (str[i++] - '0');
        }
        long x = tmp / 16;
        // writes the result of the division in the result and calculates tmp mod 16
        if (x != 0) {
                divStr[counter++] = x + '0';
        } else {
            divStr[counter++] = '0';
        }
        tmp %= 16;

    }

    divStr[counter] = '\0';

    free(str);


    return divStr;

}

// reverse a given String
char* reverseString(char* str){
    long long length = (long long) strlen(str);
    // exchanges the values until the middle is reached
    for(long long i = 0; i < length / 2; i++){
        char tmp = str[i];
        str[i] = str[length - i -1];
        str[length - i -1] = tmp;
    }
    return str;
}

// calculates a string that represents a very long mod 16
int mod16(char* str){

    // to calculate mod 16 only the last 4 digits are important, those need to have mod 16 applied to
    long long y = (long long) strlen(str);
    switch (y) {
        case 0:
            return 0;
        case 1:
            return str[y-1] - '0';
        case 2:
            return ((str[y-2] - '0') * 10 + (str[y-1] - '0')) % 16;
        case 3:
            return ((str[y-3] - '0') * 100 + (str[y-2] - '0') * 10 + (str[y-1] - '0')) % 16;
        default:
            return ((str[y-4] - '0') * 1000 + (str[y-3] - '0') * 100 +
                    (str[y-2] - '0') * 10 + (str[y-1] - '0'))  % 16;
    }

}

// converts a string that represents a decimal number into a hexadecimal number
char * intToHex(char * intStr) {

    // creates the new String
    long long length = (long long) strlen(intStr);
    char* hexString = malloc((1+length) * sizeof (char));
    if(hexString == NULL){
        printf("Speicher konnte nicht alloziert werden");
        exit(1);
    }
    int counter = 0;

    while(counter < length){
        // the result of the number mod 16 is the last digit of the hexadecimal result
        int mod = mod16(intStr);
        // converts the number into a hexadecimal number
        if(mod < 10)
            hexString[counter++] = mod + '0';
        else
            switch (mod) {
                case 10:
                    hexString[counter++] = 'A';
                    break;
                case 11:
                    hexString[counter++] = 'B';
                    break;
                case 12:
                    hexString[counter++] = 'C';
                    break;
                case 13:
                    hexString[counter++] = 'D';
                    break;
                case 14:
                    hexString[counter++] = 'E';
                    break;
                case 15:
                    hexString[counter++] = 'F';
                    break;
                default:
                    printf("\n Konvertierung in Hex String fehlerhaft! \n");
                    exit(2);
            }
        // divides the number by 16
        if(intStr != NULL) {
            intStr = div16(intStr);
        }
    }

    // Zero-terminates and reverses the string
    while (hexString[counter-1] == '0'){
        counter--;
    }
    hexString[counter] = '\0';
    hexString = reverseString(hexString);

    // removes the leading zeros
    char* a = malloc(1+counter* sizeof (char));
    if(a == NULL){
        printf("Speicher konnte nicht alloziert werden");
        exit(1);
    }
    int count = 0;
    int first = 0;
    for(int i = 0; i < counter; i++) {

        // iterates over the string until the first char is found that is not a zero
        // after that the numbers will be added to the result String
        if(first == 1) {
            a[count++] = hexString[i];
        } else {
            if(hexString[i] != '0'){
                first = 1;
                a[count++] = hexString[i];
            }
        }

    }

    // zero terminates the string
    a[counter] = '\0';
    free(hexString);
    free(intStr);
    return a;

}

// print = 1 prints the result, prints the result in hexadecimal, 0 compares the String
void squarebingum(size_t e, size_t g, int print){

    bignum matrix1,matrix2,matrix4,tmp1,tmp2,tmp3,tmp4;

    // initializes the values and set the matrix
    initbignum(&matrix1, 1);
    initbignum(&matrix2, 1);
    initbignum(&matrix4, 1);
    initbignum(&tmp1, 1);
    initbignum(&tmp2, 1);
    initbignum(&tmp3, 1);
    initbignum(&tmp4, 1);

    matrix1.numbers[0] = 0;
    matrix2.numbers[0] = 1;
    matrix4.numbers[0] = 2;
    tmp1.numbers[0] = 0;
    tmp2.numbers[0] = 0;
    tmp3.numbers[0] = 0;
    tmp4.numbers[0] = 0;

    // each iteration squares the matrix
    for(int i = 0; i < e; i++){

        setbignum(&tmp1, matrix1.numbers, matrix1.length);
        mulbignum(&tmp1, &matrix1);
        setbignum(&tmp2, matrix2.numbers, matrix2.length);
        mulbignum(&tmp2, &matrix2);
        addbignum(&tmp1, &tmp2);

        setbignum(&tmp2, matrix2.numbers, matrix2.length);
        mulbignum(&tmp2, &matrix1);
        setbignum(&tmp3, matrix2.numbers, matrix2.length);
        mulbignum(&tmp3, &matrix4);
        addbignum(&tmp2, &tmp3);

        setbignum(&tmp4, tmp2.numbers, tmp2.length);
        addbignum(&tmp4, &tmp2);
        addbignum(&tmp4, &tmp1);

        setbignum(&matrix1, tmp1.numbers, tmp1.length);
        setbignum(&matrix2, tmp2.numbers, tmp2.length);
        setbignum(&matrix4, tmp4.numbers, tmp4.length);


    }

    // executes the given print command
    if(print == 1) {
        char* str = divbignum(&matrix2, &matrix4, (long long) g, false);
        printf("Result: 1.%s\n", str);
    }
    else if(print == 2) {
        printf("Result: 1.%s\n", intToHex(divbignum(&matrix2, &matrix4, (long long) g,false)));
    }
    else if(print == 3){
        char* finalString = divbignum(&matrix2, &matrix4, (long long) g,false);
        printf("Result: 1.%s\n", finalString);
        testFunction(finalString);
    }

}
