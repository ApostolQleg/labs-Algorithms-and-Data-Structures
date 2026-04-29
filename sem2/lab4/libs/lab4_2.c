#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lab4_2.h"
#include "graph_lib.h"

int **multiply_matrices(int **A1, int **A2, int n)
{
    int **A3 = create_matrix(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                A3[i][j] += A1[i][k] * A2[k][j];
            }
        }
    }
    return A3;
}