#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph_lib.h"
#include "lab6_lib.h"

DMatrix init_dmatrix(int n)
{
    DMatrix matrix = {0};
    matrix.N = n;
    matrix.data = (double **)calloc(n, sizeof(double *));

    for (int i = 0; i < n; i++)
    {
        matrix.data[i] = (double *)calloc(n, sizeof(double));
    }

    return matrix;
}

void free_dmatrix(DMatrix *matrix)
{
    if (matrix == NULL || matrix->data == NULL)
        return;

    for (int i = 0; i < matrix->N; i++)
    {
        free(matrix->data[i]);
    }

    free(matrix->data);
    matrix->data = NULL;
    matrix->N = 0;
}

void seed_double_matrix(DMatrix *matrix)
{
    int n = matrix->N;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix->data[i][j] = randm();
        }
    }
}

void print_double_matrix(const DMatrix *matrix, const char *title)
{
    int n = matrix->N;

    printf("\n%s:\n", title);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%.3f ", matrix->data[i][j]);
        }
        printf("\n\n");
    }
}