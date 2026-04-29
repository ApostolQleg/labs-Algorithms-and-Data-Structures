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
float calc_arrow_angle(Vector2 start, Vector2 end);
Vector2 calc_arrow_target(Vector2 end, float r_node, float angle);
void draw_arrow(Vector2 target, float angle);
void draw_graph(int **matrix, Vector2 *nodes, int n, float r_node, Vector2 center, bool is_directed);

#endif