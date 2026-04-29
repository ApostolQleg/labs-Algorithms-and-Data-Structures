#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "graph_lib.h"
#include "n1_n2_n3_n4.h"

#define K (1 - (N3 * 0.02) - (N4 * 0.005) - 0.25)
#define N (10 + N3)

int main()
{
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 900;
    const int TEXT_SIZE = 20;
    const float GRAPH_RADIUS = 300.0f;
    const float NODE_RADIUS = 40.0f;

    int **A_dir = create_matrix(N);
    int **A_undir = create_matrix(N);

    srand(SEED);
    generate_directed_matrix(A_dir, N, K);
    generate_undirected_matrix(A_dir, A_undir, N);

    print_matrix(A_dir, N, "Directed Graph Matrix");
    print_matrix(A_undir, N, "Undirected Graph Matrix");

    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 3 - Graphical Graph Representation");
    SetTargetFPS(60);

    Vector2 center = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    Vector2 nodes[N];
    for (int i = 0; i < N; i++)
    {
        float angle = 2.0f * PI * i / N;
        nodes[i].x = center.x + GRAPH_RADIUS * cosf(angle);
        nodes[i].y = center.y + GRAPH_RADIUS * sinf(angle);
    }

    bool is_dir = true;
    int **current_matrix = A_dir;
    const char *title = "Directed Graph [Press 'SPACE' to switch]";

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            is_dir = !is_dir;
            current_matrix = is_dir ? A_dir : A_undir;
            title = is_dir ? "Directed Graph [Press 'SPACE' to switch]" : "Undirected Graph [Press 'SPACE' to switch]";
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        draw_graph(current_matrix, nodes, N, NODE_RADIUS, center, is_dir);

        DrawText(title, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    destroy_matrix(A_dir, N);
    destroy_matrix(A_undir, N);

    return 0;
}