#ifndef GRAPH_LIB_H
#define GRAPH_LIB_H

#include "raylib.h"
#include <stdbool.h>

typedef struct
{
    int N;
    int **data;
} IMatrix;

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

IMatrix init_imatrix(int n);
void free_imatrix(IMatrix *matrix);

double randm();
int mulmr(double value, double k);

void seed_directed_matrix(IMatrix *matrix, double k);
void seed_undirected_matrix(const IMatrix *matrix_directed, IMatrix *matrix_to_change);

void print_matrix(const IMatrix *matrix, const char *title);

void draw_arrow(Vector2 target, float angle, float arrow_length, Color color);
void draw_graph(const IMatrix *matrix, Vector2 *nodes, float r_node, Vector2 center, bool is_directed);
void draw_graph_traversal(const IMatrix *matrix, Vector2 *nodes, float r_node, Vector2 center, bool is_directed, int *visited, const IMatrix *tree_edges);

EdgeList init_edge_list();
void free_edge_list(EdgeList *list);
void add_edge(EdgeList *list, int start, int end, int weight);

void draw_weighted_edges(EdgeList *list, Vector2 *nodes, float r_node, Color edgeColor, Color textColor);

#endif