#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "graph_lib.h"
#include "n1_n2_n3_n4.h"

#define K1 (1 - (N3 * 0.01) - (N4 * 0.01) - 0.3)
#define K2 (1 - (N3 * 0.005) - (N4 * 0.005) - 0.27)
#define N (10 + N3)

int main()
{
    int **A1_dir = create_matrix(N);
    int **A1_undir = create_matrix(N);
    int **A2_dir = create_matrix(N);
    int **A2_undir = create_matrix(N);

    srand(SEED);
    A1_dir = generate_directed_matrix(A1_dir, N, K1);
    A1_undir = generate_undirected_matrix(A1_dir, A1_undir, N);

    print_matrix(A1_dir, N, "Directed Graph Matrix (K1)");
    print_matrix(A1_undir, N, "Undirected Graph Matrix (K1)");

    A2_dir = generate_directed_matrix(A2_dir, N, K2);
    A2_undir = generate_undirected_matrix(A2_dir, A2_undir, N);

    print_matrix(A2_dir, N, "Directed Graph Matrix (K2)");
    print_matrix(A2_undir, N, "Undirected Graph Matrix (K2)");

    SetTraceLogLevel(LOG_NONE);
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Lab 4 - Graph Properties and Connectivity");
    SetTargetFPS(60);

    int **show[4] = {A1_dir, A1_undir, A2_dir, A2_undir};
    int curr = 0;
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
            if (curr < 3)
            {
                curr++;
            }
            else
            {
                curr = 0;
            }
        }

        bool is_dir = (curr % 2 == 0);

        const char *dir = is_dir ? "Directed" : "Undirected";
        const char *k = curr < 2 ? "K1" : "K2";

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_graph(show[curr], nodes, N, r_node, center, is_dir);

        const char *title = TextFormat("This matrix is %s (%s) [Press 'SPACE' to switch]", dir, k);
        DrawText(title, 20, 20, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    destroy_matrix(A1_dir, N);
    destroy_matrix(A1_undir, N);
    destroy_matrix(A2_dir, N);
    destroy_matrix(A2_undir, N);

    return 0;
}