#ifndef UNTITLED1_LONGKOMMA_H
#define UNTITLED1_LONGKOMMA_H
typedef struct {
    long long * number;
    size_t length;
    size_t komma;
    // Konvention zum speichern der Zahlen
    // Die Zahl 5 1234,56789 987654321 wird in 3 Arrays gespeichert
    // [2] = 51234; [1] = 567899876 ; [0] = 543210000 ; komma = 1
    // komma bedeutet nach wie vielen Arrays die Nachkommastellen bginnen; wenn komma = length; Zahl ist ganz
    // -->  [51234] [567899876] [543210000]
    // Länge = die Anzahl der Arrays
    // --> Zahlen zischen 0 und 100.000.000 haben lenght = 1
    // -> wirkliche Länge = lengt-1 * 9 + Zifferanzahl in num[0]
} newLongKomma;

void initLongKomma(newLongKomma* num, size_t length);
void setLongKomma(newLongKomma* num, const long long * value, size_t length, size_t komma);
void printLongKomma(newLongKomma* num);
void removeZeros(newLongKomma *num);
void mulLongKomma(newLongKomma * num1, newLongKomma *num2);


#endif //UNTITLED1_LONGKOMMA_H
