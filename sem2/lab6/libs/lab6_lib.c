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
            printf("%.1f ", matrix->data[i][j]);
        }
        printf("\n\n");
    }
}

void seed_ceil_matrix(IMatrix *c_matrix, const IMatrix *imatrix, const DMatrix *dmatrix)
{
    int n = c_matrix->N;

    if (n == imatrix->N && n == dmatrix->N)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                c_matrix->data[i][j] = (int)ceil(imatrix->data[i][j] * 100 * dmatrix->data[i][j]);
            }
        }
    }
}

void seed_D_matrix(IMatrix *d_matrix, const IMatrix *c_matrix)
{
    int n = d_matrix->N;

    if (n == c_matrix->N)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                d_matrix->data[i][j] = (c_matrix->data[i][j] > 0) ? 1 : 0;
            }
        }
    }
}

void seed_H_matrix(IMatrix *h_matrix, const IMatrix *d_matrix)
{
    int n = h_matrix->N;

    if (n == d_matrix->N)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                h_matrix->data[i][j] = (d_matrix->data[i][j] != d_matrix->data[j][i]) ? 1 : 0;
                if (h_matrix->data[i][j] == 0 && ((double)rand() / RAND_MAX) < 0.25f)
                {
                    h_matrix->data[i][j] = 1; // генерація кількох випадкових одиниць
                }
            }
        }
    }
}

void seed_T_matrix(IMatrix *t_matrix)
{
    int n = t_matrix->N;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i < j)
            {
                t_matrix->data[i][j] = 1;
            }
        }
    }
}

void seed_W_matrix(IMatrix *w_matrix, const IMatrix *c_matrix, const IMatrix *d_matrix, const IMatrix *h_matrix, const IMatrix *t_matrix)
{
    int n = w_matrix->N;

    if (n == c_matrix->N && n == d_matrix->N && n == h_matrix->N && n == t_matrix->N)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i < j)
                {
                    w_matrix->data[i][j] = (d_matrix->data[i][j] + h_matrix->data[i][j] * t_matrix->data[i][j]) * c_matrix->data[i][j];
                    w_matrix->data[j][i] = (d_matrix->data[i][j] + h_matrix->data[i][j] * t_matrix->data[i][j]) * c_matrix->data[i][j];
                }
            }
        }
    }
}

EdgeList init_edge_list()
{
    EdgeList list = {NULL, 0};
    return list;
}

void add_edge(EdgeList *list, int start, int end, int weight)
{
    Edge *new_edge = (Edge *)malloc(sizeof(Edge));
    if (new_edge == NULL)
        return;

    new_edge->start = start;
    new_edge->end = end;
    new_edge->weight = weight;

    new_edge->next = list->head;
    list->head = new_edge;

    list->size++;
}

void free_edge_list(EdgeList *list)
{
    Edge *current = list->head;
    while (current != NULL)
    {
        Edge *temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
    list->size = 0;
}

EdgeList convert_w_matrix(const IMatrix *w_matrix)
{
    EdgeList list = init_edge_list();
    int n = w_matrix->N;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (w_matrix->data[i][j] > 0)
            {
                add_edge(&list, i, j, w_matrix->data[i][j]);
            }
        }
    }
    return list;
}