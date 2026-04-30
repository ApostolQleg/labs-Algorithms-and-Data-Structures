#ifndef LAB6_LIB_H
#define LAB6_LIB_H

#include "raylib.h"
#include <stdbool.h>

typedef struct
{
    int N;
    double **data;
} DMatrix;

typedef struct Edge
{
    int start;
    int end;
    int weight;
    struct Edge *next;
} Edge;

typedef struct
{
    Edge *head;
    int size;
} EdgeList;

DMatrix init_dmatrix(int n);
void free_dmatrix(DMatrix *matrix);

void seed_double_matrix(DMatrix *matrix);
void seed_ceil_matrix(IMatrix *c_matrix, const IMatrix *imatrix, const DMatrix *dmatrix);

void seed_D_matrix(IMatrix *d_matrix, const IMatrix *c_matrix);
void seed_H_matrix(IMatrix *h_matrix, const IMatrix *d_matrix);
void seed_T_matrix(IMatrix *t_matrix);
void seed_W_matrix(IMatrix *w_matrix, const IMatrix *c_matrix, const IMatrix *d_matrix, const IMatrix *h_matrix, const IMatrix *t_matrix);

EdgeList init_edge_list();
void free_edge_list(EdgeList *list);
void add_edge(EdgeList *list, int start, int end, int weight);
EdgeList convert_w_matrix(const IMatrix *w_matrix);

void print_double_matrix(const DMatrix *matrix, const char *title);

#endif