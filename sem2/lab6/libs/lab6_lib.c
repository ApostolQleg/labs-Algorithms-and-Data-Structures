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

int dsu_find(int *parent, int i)
{
    if (parent[i] == i)
        return i;
    return parent[i] = dsu_find(parent, parent[i]);
}

void dsu_union(int *parent, int i, int j)
{
    int root_i = dsu_find(parent, i);
    int root_j = dsu_find(parent, j);
    if (root_i != root_j)
    {
        parent[root_i] = root_j;
    }
}

KruskalState init_kruskal_state(EdgeList *sorted_edges, int n)
{
    KruskalState state;
    state.sorted_edges = sorted_edges;
    state.current_edge = sorted_edges->head;
    state.mst_edges = init_edge_list();
    state.total_weight = 0;
    state.finished = false;

    for (int i = 0; i < n; i++)
    {
        state.parent[i] = i;
    }
    return state;
}

void kruskal_step(KruskalState *state)
{
    if (state->finished || state->current_edge == NULL)
    {
        state->finished = true;
        return;
    }

    int start = state->current_edge->start;
    int end = state->current_edge->end;
    int weight = state->current_edge->weight;

    if (dsu_find(state->parent, start) != dsu_find(state->parent, end))
    {
        dsu_union(state->parent, start, end);
        add_edge(&state->mst_edges, start, end, weight);
        state->total_weight += weight;
    }

    state->current_edge = state->current_edge->next;
}