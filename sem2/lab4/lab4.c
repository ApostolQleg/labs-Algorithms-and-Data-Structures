#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "graph_lib.h"
#include "n1_n2_n3_n4.h"
#include "lab4_1.h"
#include "lab4_2.h"

#define K1 (1 - (N3 * 0.01) - (N4 * 0.01) - 0.3)
#define K2 (1 - (N3 * 0.005) - (N4 * 0.005) - 0.27)
#define N (10 + N3)

int main()
{
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 1200;
    const int TEXT_SIZE = 30;
    const float GRAPH_RADIUS = 250.0f;
    const float NODE_RADIUS = 30.0f;

    srand(SEED);

    int **A_dir = create_matrix(N);
    int **A_undir = create_matrix(N);
    int **B_dir = create_matrix(N);
    int **B_undir = create_matrix(N);

    generate_directed_matrix(A_dir, N, K1);
    generate_undirected_matrix(A_dir, A_undir, N);
    generate_directed_matrix(B_dir, N, K2);
    generate_undirected_matrix(B_dir, B_undir, N);

    print_matrix(A_dir, N, "Directed Graph Matrix (K1)");
    print_matrix(A_undir, N, "Undirected Graph Matrix (K1)");
    print_matrix(B_dir, N, "Directed Graph Matrix (K2)");
    print_matrix(B_undir, N, "Undirected Graph Matrix (K2)");

    int out_deg[N] = {0};
    int in_deg[N] = {0};
    int undir_deg[N] = {0};

    calc_degrees_dir(A_dir, N, out_deg, in_deg);
    calc_degrees_undir(A_undir, N, undir_deg);

    print_degrees(out_deg, N, "Out-degrees of Directed matrix (K1)");
    print_degrees(in_deg, N, "In-degrees of Directed matrix (K1)");
    print_degrees(undir_deg, N, "Degrees of Undirected matrix (K1)");
    printf("\n");

    int r_out_deg = calc_regularity(out_deg, N);
    int r_in_deg = calc_regularity(in_deg, N);
    int r_undir_deg = calc_regularity(undir_deg, N);

    print_regularity(r_out_deg, "Directed out-degree (K1)");
    print_regularity(r_in_deg, "Directed  in-degree (K1)");
    print_regularity(r_undir_deg, "Undirected (K1)");
    printf("\n");

    int p_out_deg[N] = {0};
    int p_in_deg[N] = {0};
    int p_undir_deg[N] = {0};

    int p_out_count = calc_pendant(out_deg, N, p_out_deg);
    int p_in_count = calc_pendant(in_deg, N, p_in_deg);
    int p_undir_count = calc_pendant(undir_deg, N, p_undir_deg);

    print_pendant(p_out_deg, N, p_out_count, "Directed out-degree (K1)");
    print_pendant(p_in_deg, N, p_in_count, "Directed in-degree (K1)");
    print_pendant(p_undir_deg, N, p_undir_count, "Undirected (K1)");
    printf("\n");

    int i_out_deg[N] = {0};
    int i_in_deg[N] = {0};
    int i_undir_deg[N] = {0};

    int i_out_count = calc_isolated(out_deg, N, i_out_deg);
    int i_in_count = calc_isolated(in_deg, N, i_in_deg);
    int i_undir_count = calc_isolated(undir_deg, N, i_undir_deg);

    print_isolated(i_out_deg, N, i_out_count, "Directed out-degree (K1)");
    print_isolated(i_in_deg, N, i_in_count, "Directed in-degree (K1)");
    print_isolated(i_undir_deg, N, i_undir_count, "Undirected (K1)");
    printf("\n");

    int **B_dir_2 = multiply_matrices(B_dir, B_dir, N);
    int **B_dir_3 = multiply_matrices(B_dir_2, B_dir, N);

    print_matrix(B_dir_2, N, "Directed Graph paths of length 2 (K2)");
    print_matrix(B_dir_3, N, "Directed Graph paths of length 3 (K2)");

    int **B_dir_reach = create_reach_matrix(B_dir, N);
    int **B_undir_reach = create_reach_matrix(B_undir, N);

    print_matrix(B_dir_reach, N, "Directed Graph Reachability matrix (K2)");
    print_matrix(B_undir_reach, N, "Undirected Graph Reachability matrix (K2)");

    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 4 - Graph Properties and Connectivity");
    SetTargetFPS(60);

    int **show[4] = {A_dir, A_undir, B_dir, B_undir};
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
        draw_degrees(show[curr], nodes, N, NODE_RADIUS, center, is_dir);

        const char *title = TextFormat("This matrix is %s (%s) [Press 'SPACE' to switch]", dir_text, k_text);
        DrawText(title, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    destroy_matrix(A_dir, N);
    destroy_matrix(A_undir, N);
    destroy_matrix(B_dir, N);
    destroy_matrix(B_undir, N);
    destroy_matrix(B_dir_2, N);
    destroy_matrix(B_dir_3, N);

    return 0;
}