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

void split_list(Edge *source, Edge **front, Edge **back)
{
    Edge *fast;
    Edge *slow;
    slow = source;
    fast = source->next;

    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

Edge *sorted_merge(Edge *a, Edge *b)
{
    Edge *result = NULL;

    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    if (a->weight <= b->weight)
    {
        result = a;
        result->next = sorted_merge(a->next, b);
    }
    else
    {
        result = b;
        result->next = sorted_merge(a, b->next);
    }

    return result;
}

void merge_sort_recursive(Edge **head_ref)
{
    Edge *head = *head_ref;
    Edge *a;
    Edge *b;

    if ((head == NULL) || (head->next == NULL))
    {
        return;
    }

    split_list(head, &a, &b);

    merge_sort_recursive(&a);
    merge_sort_recursive(&b);

    *head_ref = sorted_merge(a, b);
}

void sort_edge_list(EdgeList *list)
{
    if (list == NULL || list->head == NULL)
        return;

    merge_sort_recursive(&(list->head));
}

void print_edge_list(const EdgeList *list, const char *title)
{
    printf("\n%s:\n", title);

    if (list == NULL || list->head == NULL)
    {
        printf("Edge list is empty.\n\n");
        return;
    }

    Edge *current = list->head;
    int count = 1;

    while (current != NULL)
    {
        printf("%3d. Edge (%2d, %2d) | Weight: %d\n",
               count,
               current->start + 1,
               current->end + 1,
               current->weight);

        current = current->next;
        count++;
    }
    printf("There is %d edges\n\n", list->size);
}