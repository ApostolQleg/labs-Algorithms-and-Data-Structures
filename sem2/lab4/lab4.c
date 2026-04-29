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
// #define K2 (1 - (N3 * 0.005) - (N4 * 0.035) - 0.27) // - наглядніше для графа конденсації
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

    print_matrix(A_dir, N, "Directed Graph matrix (K1)");
    print_matrix(A_undir, N, "Undirected Graph matrix (K1)");
    print_matrix(B_dir, N, "Directed Graph matrix (K2)");
    print_matrix(B_undir, N, "Undirected Graph matrix (K2)");

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

    int **B_dir_strong = create_strong_connect_matrix(B_dir_reach, N);

    print_matrix(B_dir_strong, N, "Directed Graph Strong Connectivity matrix (K2)");
    print_strong_groups(B_dir_strong, N, "Directed Graph (K2)");

    int groups[N] = {0};
    int count = 0;

    int **B_dir_cond = create_condensation_matrix(B_dir, B_dir_strong, N, &count, groups);

    print_matrix(B_dir_cond, count, "Directed Graph Condensation matrix (K2)");

    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 4 - Graph Properties and Connectivity");
    SetTargetFPS(60);

    int **show_matrices[5] = {A_dir, A_undir, B_dir, B_undir, B_dir_cond};
    int show_sizes[5] = {N, N, N, N, count};
    bool show_is_dir[5] = {true, false, true, false, true};
    const char *show_titles[5] = {
        "Directed (K1)",
        "Undirected (K1)",
        "Directed (K2)",
        "Undirected (K2)",
        "Condensation Graph (K2)"};

    int curr = 0;
    Vector2 center = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            curr = (curr + 1) % 5;
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

        draw_degrees(current_matrix, current_nodes, current_n, NODE_RADIUS, center, is_dir);

        const char *title = TextFormat("Showing: %s [Press 'SPACE' to switch]", show_titles[curr]);
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

    destroy_matrix(B_dir_reach, N);
    destroy_matrix(B_undir_reach, N);

    destroy_matrix(B_dir_strong, N);
    destroy_matrix(B_dir_cond, count);

    return 0;
}