#ifndef GRAPH_LIB_H
#define GRAPH_LIB_H

#include "raylib.h"
#include <stdbool.h>

int **create_matrix(int n);
void destroy_matrix(int **matrix, int n);

double **create_double_matrix(int n);
void destroy_double_matrix(double **matrix, int n);

double randm();
int mulmr(double value, double k);

void seed_directed_matrix(int **matrix, int n, double k);
void seed_undirected_matrix(int **matrix_directed, int **matrix_to_change, int n);
void seed_double_matrix(double **matrix, int n);

void print_matrix(int **matrix, int n, const char *title);
void print_double_matrix(double **matrix, int n, const char *title);

void draw_arrow(Vector2 target, float angle, float arrow_length, Color color);
void draw_graph(int **matrix, Vector2 *nodes, int n, float r_node, Vector2 center, bool is_directed);
void draw_graph_traversal(int **matrix, Vector2 *nodes, int n, float r_node, Vector2 center, bool is_directed, int *visited, int **tree_edges);

#endif