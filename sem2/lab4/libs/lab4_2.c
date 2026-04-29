#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lab4_2.h"
#include "graph_lib.h"

int **multiply_matrices(int **matrix_1, int **matrix_2, int n)
{
    int **matrix = create_matrix(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                matrix[i][j] += matrix_1[i][k] * matrix_2[k][j];
            }
        }
    }
    return matrix;
}

int **create_reach_matrix(int **matrix, int n)
{
    int **r_matrix = create_matrix(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            r_matrix[i][j] = (matrix[i][j] != 0) || (i == j);
        }
    }

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                r_matrix[i][j] = r_matrix[i][j] || (r_matrix[i][k] && r_matrix[k][j]);
            }
        }
    }

    return r_matrix;
}

int **create_strong_connect_matrix(int **r_matrix, int n)
{
    int **s_matrix = create_matrix(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            s_matrix[i][j] = r_matrix[i][j] && r_matrix[j][i];
        }
    }
    return s_matrix;
}

void print_strong_groups(int **s_matrix, int n, char *graph_name)
{
    int *visited = (int *)calloc(n, sizeof(int));
    int count = 0;

    printf("\nStrongly Connected Components for %s:\n", graph_name);

    for (int i = 0; i < n; i++)
    {
        if (!visited[i])
        {
            count++;
            printf("Component %d: { %d", count, i + 1);
            visited[i] = 1;

            for (int j = i + 1; j < n; j++)
            {
                if (s_matrix[i][j] == 1)
                {
                    printf(", %d", j + 1);
                    visited[j] = 1;
                }
            }
            printf(" }\n");
        }
    }

    free(visited);
}