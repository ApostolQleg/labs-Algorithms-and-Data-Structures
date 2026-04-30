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
void seed_ceil_cmatrix(IMatrix *c_matrix, const IMatrix *imatrix, const DMatrix *dmatrix);

void seed_D_matrix(IMatrix *d_matrix, const IMatrix *c_matrix);
void seed_H_matrix(IMatrix *h_matrix, const IMatrix *d_matrix);
void seed_T_matrix(IMatrix *t_matrix);

void print_double_matrix(const DMatrix *matrix, const char *title);

#endif