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
void seed_ceil_matrix(IMatrix *c_matrix, const IMatrix *imatrix, const DMatrix *dmatrix);

void seed_D_matrix(IMatrix *d_matrix, const IMatrix *c_matrix);
void seed_H_matrix(IMatrix *h_matrix, const IMatrix *d_matrix);
void seed_T_matrix(IMatrix *t_matrix);
void seed_W_matrix(IMatrix *w_matrix, const IMatrix *c_matrix, const IMatrix *d_matrix, const IMatrix *h_matrix, const IMatrix *t_matrix);

EdgeList convert_w_matrix(const IMatrix *w_matrix);

void split_list(Edge *source, Edge **front, Edge **back);
Edge *sorted_merge(Edge *a, Edge *b);
void merge_sort_recursive(Edge **head_ref);
void sort_edge_list(EdgeList *list);
void print_edge_list(const EdgeList *list, const char *title);

void print_double_matrix(const DMatrix *matrix, const char *title);

typedef struct
{
    EdgeList *sorted_edges;
    Edge *current_edge;
    EdgeList mst_edges;
    int parent[100];
    int total_weight;
    bool finished;
} KruskalState;

KruskalState init_kruskal_state(EdgeList *sorted_edges, int n);
void kruskal_step(KruskalState *state);

#endif