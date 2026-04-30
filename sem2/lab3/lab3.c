#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "graph_lib.h"
#include "n1_n2_n3_n4.h"

#define K (1 - (N3 * 0.02) - (N4 * 0.005) - 0.25)
#define GRAPH_N (10 + N3)

int main()
{
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 1200;
    const int TEXT_SIZE = 30;
    const float GRAPH_RADIUS = 350.0f;
    const float NODE_RADIUS = 40.0f;

    IMatrix A_dir = init_imatrix(GRAPH_N);
    IMatrix A_undir = init_imatrix(GRAPH_N);

    srand(SEED);

    seed_directed_matrix(&A_dir, K);
    seed_undirected_matrix(&A_dir, &A_undir);

    print_matrix(&A_dir, "Directed Graph Matrix");
    print_matrix(&A_undir, "Undirected Graph Matrix");

    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 3 - Graphical Graph Representation");
    SetTargetFPS(60);

    IMatrix *show_matrices[2] = {&A_dir, &A_undir};
    bool show_is_dir[2] = {true, false};
    const char *show_titles[2] = {"Directed", "Undirected"};

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
    free_imatrix(&A_undir);

    return 0;
}