#ifndef LAB4_1_H
#define LAB4_1_H

#include "raylib.h"
#include <stdbool.h>
#include "graph_lib.h"

void calc_degrees_dir(const IMatrix *matrix, int *out_degrees, int *in_degrees);
void calc_degrees_undir(const IMatrix *matrix, int *undir_degrees);
void print_degrees(int *degrees, int n, char *message);
void draw_degrees(const IMatrix *matrix, Vector2 *nodes, float r_node, Vector2 center, bool is_dir);
int calc_regularity(int *degrees, int n);
void print_regularity(int r_degree, char *graph_name);
int calc_pendant(int *degrees, int n, int *p_deg);
void print_pendant(int *p_deg, int n, int count, char *graph_name);
int calc_isolated(int *degrees, int n, int *i_deg);
void print_isolated(int *i_deg, int n, int count, char *graph_name);

#endif