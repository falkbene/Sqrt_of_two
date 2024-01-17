#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "getopt.h"
#include "Expo.h"
#include "Bignum.h"
#include "IntArr.h"
#include <time.h>
#include "errno.h"

void help(){    //Function to provide more information about the functionality of the program
    printf("This program calculates sqrt(2) and depending on the command-line argument prints the numbers out as"
           "Decimal or Hexadecimal. \nYou have the following options: -V (with the optional argument of the Implementation you want "
           "to choose). There are 5 options for the Version of the program:\n"
           "0) Exponentiation by squaring coupled with long-array datastructures and SIMD\n"
           "1) Self-developed algorithm for exponentiation with long-array datastructures\n"
           "2) Exponentiation by squaring coupled with int-array datastructures\n"
           "3) Self-developed algorithm for exponentiation with int-array datastructures\n"
           "4) Exponentiation by squaring with long-array datastructures without the usage of SIMD\n"
           "The Option -B measures the time it takes to produce a result with the optional argument of repetitions of the function \n"
           "The Option -d returns the sqrt(2) in decimal Radix it has priority over -h with returns the numbers as hexadecimal representation \n"
           "both options take an argument which is the amount of places you want to have printed out\n"
           "The Option -t takes a string as an optional argument and tests how many decimal places correspond to the squareroot of two (up to 10Million).\n"
           "If no argument is provided it compares the calculate result to the correct decimal string and doesn't return the string. It does not work with the hexadecimal representation \n"
           "-h without an argument displays this message the same goes for --help \n"
           "Last but not least please consider that large precision (>100000) can take some time to compute and especially converting the result to hex-representation may take time \n");
}
void error_exit(const char* message) {      //in case of an error occurring during execution of main.c
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}
long long parseLongFromCommandline(char* opt_arg) {        //Method to check for invalid arguments
    long long arg = 0;
    errno = 0;
    char *endptr;
    arg = strtol(opt_arg, &endptr, 10);
    if((errno == ERANGE && (arg == __LONG_MAX__ || arg == (-(__LONG_MAX__) -1L)))|| (errno != 0 && arg == 0)) {
        // arg is valid
        error_exit("The Value provided for the amount of repetitions of the function was not provided correctly!\n");
    }
    else if(endptr == opt_arg) {
        error_exit("No digits were found! \n");
    }
    else if(*endptr != '\0') {
        error_exit("Please refrain from appending non-integers to your numbers!\n");
    }
    return arg;
}


int main(int argc, char** argv) {
    int opt;
    char* testString = NULL;
    long long implementation = 0;  // Set default implementation
    bool measure = false;
    long long repetition = 1;      //Default amount of repetitions
    bool has_h = false;
    bool has_d = false;
    size_t h_arg = -1;  // for hexadecimal places
    size_t d_arg = -1;  // for decimal places
    errno = 0;
    bool test = false;
    static struct option long_options[] = {
            {"help", no_argument, 0, 'H'},
            {0, 0, 0, 0}
    };      //to be able to parse --help
    int optionId  = 0;
    while ((opt = getopt_long(argc, argv, "V::B::d:h::t::", long_options, &optionId)) != -1) {
        optionId  = 0;
        switch (opt) {
            case 'H': //--help
                help();
                return EXIT_SUCCESS;
            case 'V':   //Version chosen
                if (optarg != NULL) {
                    implementation = parseLongFromCommandline(optarg);
                    if(implementation>4||implementation<0){
                        error_exit("Please provide a valid version, for more information run the program with --help or -h");
                    }
                }
                break;
            case 'B':       //Performance measuring? and possible repetition of the function call
                if (optarg != NULL) {
                    repetition = parseLongFromCommandline(optarg);
                }
                if(repetition<0) {
                    error_exit("Please provide a number >0 for the amount of repetitions!");
                }
                if(repetition>20) {
                    printf("Depending on your provided accuracy this amount of function calls may take some time...\n");
                }
                measure = true;
                break;
            case 'd':       //decimal output
                if (optarg != NULL) {
                    d_arg = parseLongFromCommandline(optarg);
                    if(d_arg > 100000) {
                        printf("This might take some time to compute do not abort if you want the result ... \n");
                    }
                    has_d = true;
                }
                break;
            case 'h':   //hexadecimal output or help
                if (optarg == NULL) {
                    help();
                    exit(EXIT_SUCCESS);
                } else {
                    h_arg = parseLongFromCommandline(optarg);
                    if(h_arg > 10000) {
                        printf("This might take some time to compute do not abort if you want the result ... \n");
                    }
                    has_h = true;
                }
                break;
            case 't':       //Test the optional inputString on equality to square-root of two
                if(optarg!=NULL) {
                    testString = optarg;
                }
                test = true;
                break;
            case '?':
                // Handle unrecognized options or missing arguments
                error_exit("Unrecognized option or missing arguments!");
        }
    }

    if(!has_h && !has_d) {          //check if a precision is specified
        if(test && testString!=NULL) {
            testFunction(testString);
            return EXIT_SUCCESS;
        }
        error_exit("No precision specified! Please provide either -d or -h!");
    }
    size_t gen;

    if(has_d) {         //Find the precision
        gen = d_arg;

    } else{
        gen = h_arg;
    }

    //Reference for print: 1 => Return the result in decimal representation
    //                     2 => Return the result in hexadecimal representation
    //                     3 => Compare the string with sqrt(2) up to 10 Million digits
    int print;   //Find the right return Option
    if (test) {
        print = 3;
        if(testString!=NULL) {
            error_exit("Please don't provide a value to test and demand a calculation at the same time! ");
        }
        if(has_h && !has_d) {
            error_exit("Testing the result doesn't work with hexadecimal! ");
        }
    }
    else if(has_d) {
        print = 1;
    }
    else if(has_h) {
        print = 2;
    }

    size_t wert = ceil(1.37*(double) gen);  //calculate the amount of squaring in exponentiation by squaring
    size_t exponentiations = ceil(log((double) gen)/ log(4)); //calculate the amount of iterations on the loop
    exponentiations = exponentiations * 2 +1;
    if(wert== 0 && gen>0) {
        error_exit("Sorry but the maximum of possible accuracy has been reached or you chose a negative Number! \n");
    }
    if(implementation == 0) {
        //Mainimplementation with squared exponentiation
        if(measure) {
            clock_t begin, end;
            begin = clock();
            for(int i = 0;i<repetition;i++) {
                expo(wert,gen,print,true);       //Exponentiation by squaring Algorithm
            }
            end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

            printf("\nThe elapsed time is %f seconds\n", time_spent);
        }
        else {
            expo(wert,gen,print,true);
        }
    } else if (implementation == 1) {
        if(measure) {
            clock_t begin, end;
            begin = clock();
            for(int i = 0;i<repetition;i++) {
                squarebingum(exponentiations, gen, print);        //self developed algorithm
            }
            end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("\nThe elapsed time is %f seconds\n", time_spent);
        }
        else {
            squarebingum(exponentiations, gen, print);
        }
    }
    else if (implementation == 2) {
        if(measure) {
            clock_t begin, end;
            begin = clock();
            for(int i = 0;i<repetition;i++) {
                expoInt(wert,gen,print);        //squared exponentiation with int-datastructures
            }
            end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("\nThe elapsed time is %f seconds\n", time_spent);
        }
        else {
            expoInt(wert,gen,print);
        }
    }
    else if(implementation == 3) {
        if(measure) {
            clock_t begin, end;
            begin = clock();
            for(int i = 0;i<repetition;i++) {
                exp1Int(exponentiations,gen,print);        //self developed algorithm with int
            }
            end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("\nThe elapsed time is %f seconds\n", time_spent);
        }
        else {
            exp1Int(exponentiations, gen, print);
        }
    }
    else if(implementation == 4) {
        if(measure) {
            clock_t begin, end;
            begin = clock();
            for(int i = 0;i<repetition;i++) {
                expo(wert,gen,print,false);       //Exponentiation by squaring Algorithm
            }
            end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("\nThe elapsed time is %f seconds\n", time_spent);
        }
        else {
            expo(wert,gen,print,false);
        }
    }
    return EXIT_SUCCESS;
}

