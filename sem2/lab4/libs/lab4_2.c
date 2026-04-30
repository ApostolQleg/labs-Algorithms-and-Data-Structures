#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lab4_2.h"

IMatrix multiply_matrices(const IMatrix *matrix_1, const IMatrix *matrix_2)
{
    int n = matrix_1->N;
    IMatrix matrix = init_imatrix(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                matrix.data[i][j] += matrix_1->data[i][k] * matrix_2->data[k][j];
            }
        }
    }
    return matrix;
}

IMatrix create_reach_matrix(const IMatrix *matrix)
{
    int n = matrix->N;
    IMatrix r_matrix = init_imatrix(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            r_matrix.data[i][j] = (matrix->data[i][j] != 0) || (i == j);
        }
    }

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                r_matrix.data[i][j] = r_matrix.data[i][j] || (r_matrix.data[i][k] && r_matrix.data[k][j]);
            }
        }
    }

    return r_matrix;
}

IMatrix create_strong_connect_matrix(const IMatrix *r_matrix)
{
    int n = r_matrix->N;
    IMatrix s_matrix = init_imatrix(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            s_matrix.data[i][j] = r_matrix->data[i][j] && r_matrix->data[j][i];
        }
    }
    return s_matrix;
}

void print_strong_groups(const IMatrix *s_matrix, char *graph_name)
{
    int n = s_matrix->N;
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
                if (s_matrix->data[i][j] == 1)
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

IMatrix create_condensation_matrix(const IMatrix *matrix, const IMatrix *s_matrix, int *count, int *groups)
{
    int n = matrix->N;
    int *visited = (int *)calloc(n, sizeof(int));
    int comp_id = 0;

    for (int i = 0; i < n; i++)
    {
        if (!visited[i])
        {
            groups[i] = comp_id;
            visited[i] = 1;

            for (int j = i + 1; j < n; j++)
            {
                if (s_matrix->data[i][j] == 1)
                {
                    groups[j] = comp_id;
                    visited[j] = 1;
                }
            }
            comp_id++;
        }
    }
    free(visited);

    *count = comp_id;

    IMatrix cond_matrix = init_imatrix(comp_id);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix->data[i][j] == 1)
            {
                int comp_i = groups[i];
                int comp_j = groups[j];

                if (comp_i != comp_j)
                {
                    cond_matrix.data[comp_i][comp_j] = 1;
                }
            }
        }
    }

    return cond_matrix;
}