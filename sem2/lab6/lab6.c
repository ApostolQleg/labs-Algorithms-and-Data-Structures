#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "graph_lib.h"
#include "n1_n2_n3_n4.h"
#include "lab6_lib.h"

#define K (1 - (N3 * 0.01) - (N4 * 0.005) - 0.05)
#define GRAPH_N (10 + N3)

int main()
{
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 1200;
    const int TEXT_SIZE = 30;
    const float GRAPH_RADIUS = 350.0f;
    const float NODE_RADIUS = 40.0f;

    IMatrix A_dir = init_imatrix(GRAPH_N);
    IMatrix A = init_imatrix(GRAPH_N);
    DMatrix B = init_dmatrix(GRAPH_N);
    IMatrix C = init_imatrix(GRAPH_N);
    IMatrix D = init_imatrix(GRAPH_N);
    IMatrix H = init_imatrix(GRAPH_N);
    IMatrix T = init_imatrix(GRAPH_N);

    srand(SEED);

    seed_directed_matrix(&A_dir, K);
    seed_undirected_matrix(&A_dir, &A);

    seed_double_matrix(&B);
    seed_ceil_cmatrix(&C, &A, &B); // В методичці так, але тоді зануляється H

    seed_D_matrix(&D, &C);
    seed_H_matrix(&H, &D);
    seed_T_matrix(&T);

    print_matrix(&A, "Undirected Graph Matrix");
    print_double_matrix(&B, "Random Graph Matrix");

    print_matrix(&C, "Ceiled Matrix (c_ij = b_ij * 100 * a_ij)");
    print_matrix(&D, "D Matrix (d_ij = 1 if c_ij > 0 and 0 if c_ij = 0)");
    print_matrix(&H, "H Matrix (h_ij = 1 if d_ij != d_ji)");
    print_matrix(&T, "Trianglular upper matrix");

    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 6 - Minimum Spanning Tree");
    SetTargetFPS(60);

    IMatrix *show_matrices[2] = {&A, &A};
    bool show_is_dir[2] = {false, false};
    const char *show_titles[2] = {"Undirected", "Undirected"};

    int curr = 0;
    Vector2 center = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            curr = (curr + 1) % 2;
        }

        IMatrix *current_matrix = show_matrices[curr];
        int current_n = current_matrix->N;
        bool is_dir = show_is_dir[curr];

        Vector2 current_nodes[current_n];
        for (int i = 0; i < current_n; i++)
        {
            float angle = 2.0f * PI * i / current_n;
            current_nodes[i].x = center.x + GRAPH_RADIUS * cosf(angle);
            current_nodes[i].y = center.y + GRAPH_RADIUS * sinf(angle);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        draw_graph(current_matrix, current_nodes, NODE_RADIUS, center, is_dir);

        const char *title = TextFormat("Showing: %s [Press 'SPACE' to switch]", show_titles[curr]);
        DrawText(title, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();

    free_imatrix(&A_dir);
    free_imatrix(&A);
    free_dmatrix(&B);
    free_imatrix(&C);

    return 0;
}