#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "graph_lib.h"
#include "n1_n2_n3_n4.h"

#define K (1 - (N3 * 0.01) - (N4 * 0.005) - 0.05)
#define N (10 + N3)

int main()
{
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 1200;
    const int TEXT_SIZE = 30;
    const float GRAPH_RADIUS = 350.0f;
    const float NODE_RADIUS = 40.0f;

    int **A_dir = create_matrix(N);
    int **A_undir = create_matrix(N);

    srand(SEED);
    generate_directed_matrix(A_dir, N, K);
    generate_undirected_matrix(A_dir, A_undir, N);

    print_matrix(A_undir, N, "Undirected Graph Matrix");

    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 6 - Minimum Spanning Tree");
    SetTargetFPS(60);

    int **show_matrices[2] = {A_undir, A_undir};
    int show_sizes[2] = {N, N};
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

        int current_n = show_sizes[curr];
        int **current_matrix = show_matrices[curr];
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

        draw_graph(current_matrix, current_nodes, current_n, NODE_RADIUS, center, is_dir);

        const char *title = TextFormat("Showing: %s [Press 'SPACE' to switch]", show_titles[curr]);
        DrawText(title, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();

    destroy_matrix(A_dir, N);
    destroy_matrix(A_undir, N);

    return 0;
}