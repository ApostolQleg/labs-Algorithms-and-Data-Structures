#ifndef LAB4_2_H
#define LAB4_2_H

int **multiply_matrices(int **A1, int **A2, int n);
int **create_reach_matrix(int **matrix, int n);
int **create_strong_connect_matrix(int **r_matrix, int n);
void print_strong_groups(int **s_matrix, int n, char *graph_name);
int **create_condensation_matrix(int **matrix, int **s_matrix, int n, int *count, int *groups);

#endif