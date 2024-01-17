#include <stdio.h>
#include <math.h>
#include "Bignum.h"
#include "LongKomma.h"
void newtonLong(bignum * N, bignum* D) { //TODO WARNING doesn't work
    bignum divisor;
    initbignum(&divisor, N->length);
    divisor.numbers = N->numbers;
    bignum quotient;
    initbignum(&quotient, D->length);
    quotient.numbers = D->numbers;
    //k = Anzahl der Bits
    unsigned long long k = divisor.length * sizeof (long long) + quotient.length*sizeof (long long);
    bignum two;
    bignum twoMul;
    initbignum(&two, 1);
    initbignum(&twoMul, 1);
    twoMul.numbers[0] = 2;
    two.numbers[0] = 1;
    for(long long i = 0;i<k;i++) {
        mulbignum(&two, &twoMul);
    }
    printf("Two: ");
    printbignum(&two);
    printf("\n");
    bignum X;
    initbignum(&X, 1);
    X.numbers[0] = 14;

    printf("X zuvor: ");
    printbignum(&X);
    printf("\n");
    bignum X_prime;
    initbignum(&X_prime, 1);
    X_prime.numbers[0] = 0;
    bignum xx;
    initbignum(&xx, 1);
    xx.numbers[0] = 0;
    int count = 0;
    while (count<10) {
        count++;
        bignum  t1; //Tempvariable for X
        initbignum(&t1, X.length);
        setbignum(&t1, X.numbers, X.length);

        bignum t2; //Tempvariable for two
        initbignum(&t2, two.length);
        setbignum(&t2, two.numbers, two.length);

        subbignum(&t2,&t1);
        mulbignum(&X, &t2);
        //nach rechts shiften
        for(long long i=0;i<k;i++){
            divbignum(&X, &twoMul, X.length,false);
        }
        printf("X: ");
        printbignum(&X);
        printf("\n");

    }
    mulbignum(&quotient, &X);
    for(long long i = 0;i<k;i++) {
        divbignum(&quotient, &twoMul, quotient.length,false);
    }
    printf("Quotient: ");
    printbignum(&quotient);
    printf("\n");

}
void calculateMandE(double D, double *M, int *e) {
    *M = frexp(D, e);  // Extract M and e from D

    *M *= 2;  // Adjust M to satisfy 1 ≤ M < 2
    *e -= 1;  // Adjust e to reflect the multiplication by 2
    printf("%f\n", *M);
    printf("%d\n",*e);
}

double computeResult(double D, double N, int P) { //works but only for Doubles
    double M_D, M_N;
    int e_D, e_N;
    calculateMandE(D, &M_D, &e_D);
    calculateMandE(N, &M_N, &e_N);

    double D_prime = D / pow(2, e_D + 1);
    //Equivalent to shifting right e_D + 1 times
    double N_prime = N / pow(2, e_D + 1);     //Normalize N and D to be between 0.5 and 1.0 for easier calculation

    double X = 48./17 - 32./17 * D_prime;
    printf("N': %f\n", N_prime);
    printf("D': %f\n", D_prime);
    int iterations = ceil(log2(P + 1) / log2(17)); //Amount of Iterations to be precise

    for (int i = 0; i < iterations; i++) {
        X = X* (2-D_prime*X);
    }
    //0.014285714285714285

    printf("X: %f\n", X);
    return N_prime * X;
}

double goldschmidtDivision(double N, double D, int maxIterations) { //Works but only for doubles
    // Scale dividend and divisor
    while (D >= 1) {
        N /= 2;
        D /= 2;
    }

    double F = 2 - D;  // Initial multiplication factor

    // Initialize scaled dividend and divisor
    double dividend = N;
    double divisor = D;

    for (int i = 0; i < maxIterations; i++) {
        dividend *= F;
        divisor *= F;

        F = 2 - divisor;

        // Check if divisor is sufficiently close to 1
        if (fabs(1 - divisor) < 1e-9) {
            break;
        }
    }

    return dividend;
}
void newtonKomma(newLongKomma* D, newLongKomma* N) {    //TODO WARNING!!! It is not functional!
    //Scale down both the divisor and the quotient
    newLongKomma D_prime, N_prime, one,X, two;
    initLongKomma(&two,1);
    initLongKomma(&X,2);
    initLongKomma(&one,1);
    initLongKomma(&D_prime,D->length);
    initLongKomma(&N_prime,N->length);
    two.number[0] = 2;
    two.komma = 1;
    one.number[0] = 1;
    one.komma = 1;
    D_prime.number = D->number;
    N_prime.number = N->number;
    D_prime.komma = 0;
    N_prime.komma = 0;
    X.number[0] = 4;
    X.number[1] = 1;
    X.komma = 1;
    //Jetzt noch durch 2 teilen bis kleiner 1
    //Tempvar for D_prime

    for(int i = 0;i<10;i++) {
        newLongKomma temp;
        initLongKomma(&temp,D_prime.length);
        temp.number = D_prime.number;
        temp.komma = D_prime.komma;
        initLongKomma(&two,1);
        two.number[0] = 2;
        two.komma = 1;
        mulLongKomma(&temp,&X); //X*(2-D_prime*X)
    }
    mulLongKomma(&N_prime,&X);
}

