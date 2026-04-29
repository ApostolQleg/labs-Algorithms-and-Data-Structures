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
    int **A_dir = create_matrix(N);
    int **A_undir = create_matrix(N);

    srand(SEED);
    A_dir = generate_directed_matrix(A_dir, N, K);
    A_undir = generate_undirected_matrix(A_dir, A_undir, N);

    print_matrix(A_dir, N, "Directed Graph Matrix");
    print_matrix(A_undir, N, "Undirected Graph Matrix");

    SetTraceLogLevel(LOG_NONE);
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Lab 3 - Raylib Graph Representation");
    SetTargetFPS(60);

    bool show_directed = true;
    float R = 300.0f;
    float r_node = 40.0f;
    Vector2 center = {screenWidth / 2.0f, screenHeight / 2.0f};

    Vector2 nodes[N];
    for (int i = 0; i < N; i++)
    {
        float angle = 2.0f * PI * i / N;
        nodes[i].x = center.x + R * cosf(angle);
        nodes[i].y = center.y + R * sinf(angle);
    }

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            show_directed = !show_directed;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        int **current_matrix = show_directed ? A_dir : A_undir;
        draw_graph(current_matrix, nodes, N, r_node, center, show_directed);

        const char *title = show_directed ? "Directed Graph (Press 'SPACE' to switch)" : "Undirected Graph (Press 'SPACE' to switch)";
        DrawText(title, 20, 20, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    destroy_matrix(A_dir, N);
    destroy_matrix(A_undir, N);

    return 0;
}