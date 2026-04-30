#ifndef LAB6_LIB_H
#define LAB6_LIB_H

#include "raylib.h"
#include <stdbool.h>

typedef struct
{
    int N;
    double **data;
} DMatrix;

DMatrix init_dmatrix(int n);
void free_dmatrix(DMatrix *matrix);

void seed_double_matrix(DMatrix *matrix);

void print_double_matrix(const DMatrix *matrix, const char *title);

#endif