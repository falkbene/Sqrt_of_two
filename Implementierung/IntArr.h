//
// Created by Fabian Strobl on 22.06.23.
//


#ifndef UNTITLED1_INTARR_H
#define UNTITLED1_INTARR_H

typedef struct {
    int * number;
    size_t length;
} intArr;

void initInt(intArr * num, size_t length);
void setInt(intArr* num, const int * value, size_t length);
void mulInt(intArr * num1, intArr *num2);
void addInt(intArr *num1, intArr *num2);
char* divLongInt(intArr * num1, intArr *num2, long long pre);
void exp1Int(size_t e, size_t g, int print);
// Funktionen um 2 bignums zu vergleichen

// Funktionen um mit bignums zu rechnen


// Funktionen um bignums zu printen




#endif //UNTITLED1_INTARR_H

