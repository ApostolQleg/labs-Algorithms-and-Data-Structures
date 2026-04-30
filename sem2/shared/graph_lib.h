#ifndef GRAPH_LIB_H
#define GRAPH_LIB_H

#include "raylib.h"
#include <stdbool.h>

typedef struct
{
    int N;
    int **data;
} IMatrix;

typedef struct
{
    int N;
    double **data;
} DMatrix;

IMatrix init_imatrix(int n);
void free_imatrix(IMatrix *matrix);

DMatrix init_dmatrix(int n);
void free_dmatrix(DMatrix *matrix);

double randm();
int mulmr(double value, double k);

void seed_directed_matrix(IMatrix *matrix, double k);
void seed_undirected_matrix(const IMatrix *matrix_directed, IMatrix *matrix_to_change);
void seed_double_matrix(DMatrix *matrix);

void print_matrix(const IMatrix *matrix, const char *title);
void print_double_matrix(const DMatrix *matrix, const char *title);

void draw_arrow(Vector2 target, float angle, float arrow_length, Color color);
void draw_graph(const IMatrix *matrix, Vector2 *nodes, float r_node, Vector2 center, bool is_directed);
void draw_graph_traversal(const IMatrix *matrix, Vector2 *nodes, float r_node, Vector2 center, bool is_directed, int *visited, const IMatrix *tree_edges);

#endif