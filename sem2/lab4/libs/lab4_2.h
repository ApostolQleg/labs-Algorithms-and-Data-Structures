#ifndef LAB4_2_H
#define LAB4_2_H

#include "graph_lib.h"

IMatrix multiply_matrices(const IMatrix *A1, const IMatrix *A2);
IMatrix create_reach_matrix(const IMatrix *matrix);
IMatrix create_strong_connect_matrix(const IMatrix *r_matrix);
void print_strong_groups(const IMatrix *s_matrix, char *graph_name);
IMatrix create_condensation_matrix(const IMatrix *matrix, const IMatrix *s_matrix, int *count, int *groups);

#endif