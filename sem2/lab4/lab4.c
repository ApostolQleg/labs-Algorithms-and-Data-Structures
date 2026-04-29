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
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 900;
    const int TEXT_SIZE = 30;
    const float GRAPH_RADIUS = 300.0f;
    const float NODE_RADIUS = 40.0f;

    int **A1_dir = create_matrix(N);
    int **A1_undir = create_matrix(N);
    int **A2_dir = create_matrix(N);
    int **A2_undir = create_matrix(N);

    srand(SEED);
    generate_directed_matrix(A1_dir, N, K1);
    generate_undirected_matrix(A1_dir, A1_undir, N);

    print_matrix(A1_dir, N, "Directed Graph Matrix (K1)");
    print_matrix(A1_undir, N, "Undirected Graph Matrix (K1)");

    generate_directed_matrix(A2_dir, N, K2);
    generate_undirected_matrix(A2_dir, A2_undir, N);

    print_matrix(A2_dir, N, "Directed Graph Matrix (K2)");
    print_matrix(A2_undir, N, "Undirected Graph Matrix (K2)");

    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 4 - Graph Properties and Connectivity");
    SetTargetFPS(60);

    int **show[4] = {A1_dir, A1_undir, A2_dir, A2_undir};
    int curr = 0;
    Vector2 center = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};

    Vector2 nodes[N];
    for (int i = 0; i < N; i++)
    {
        float angle = 2.0f * PI * i / N;
        nodes[i].x = center.x + GRAPH_RADIUS * cosf(angle);
        nodes[i].y = center.y + GRAPH_RADIUS * sinf(angle);
    }

    bool is_dir = true;
    const char *dir_text = "Directed";
    const char *k_text = "K1";

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            curr = (curr + 1) % 4;

            is_dir = (curr % 2 == 0);
            dir_text = is_dir ? "Directed" : "Undirected";
            k_text = curr < 2 ? "K1" : "K2";
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        draw_graph(show[curr], nodes, N, NODE_RADIUS, center, is_dir);

        const char *title = TextFormat("This matrix is %s (%s) [Press 'SPACE' to switch]", dir_text, k_text);
        DrawText(title, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    destroy_matrix(A1_dir, N);
    destroy_matrix(A1_undir, N);
    destroy_matrix(A2_dir, N);
    destroy_matrix(A2_undir, N);

    return 0;
}