#ifndef GRAPH_LIB_H
#define GRAPH_LIB_H

#include "raylib.h"
#include <stdbool.h>

int **create_matrix(int n);
void destroy_matrix(int **matrix, int n);
double randm();
int mulmr(double value, double k);
void generate_directed_matrix(int **matrix, int n, double k);
void generate_undirected_matrix(int **matrix_directed, int **matrix_to_change, int n);
void print_matrix(int **matrix, int n, const char *title);
void draw_arrow(Vector2 target, float angle, float arrow_length, Color color);
void draw_graph(int **matrix, Vector2 *nodes, int n, float r_node, Vector2 center, bool is_directed);

#endif