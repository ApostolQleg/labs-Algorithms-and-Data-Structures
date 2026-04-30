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
    IMatrix W = init_imatrix(GRAPH_N);

    srand(SEED);

    seed_directed_matrix(&A_dir, K);
    seed_undirected_matrix(&A_dir, &A);
    seed_double_matrix(&B);
    seed_ceil_matrix(&C, &A, &B);
    seed_D_matrix(&D, &C);
    seed_H_matrix(&H, &D);
    seed_T_matrix(&T);
    seed_W_matrix(&W, &C, &D, &H, &T);

    EdgeList all_edges = convert_w_matrix(&W);
    sort_edge_list(&all_edges);

    KruskalState k_state = init_kruskal_state(&all_edges, GRAPH_N);

    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 6 - Kruskal's Minimum Spanning Tree");
    SetTargetFPS(60);

    Vector2 center = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    Vector2 current_nodes[GRAPH_N];

    for (int i = 0; i < GRAPH_N; i++)
    {
        float angle = 2.0f * PI * i / GRAPH_N;
        current_nodes[i].x = center.x + GRAPH_RADIUS * cosf(angle);
        current_nodes[i].y = center.y + GRAPH_RADIUS * sinf(angle);
    }

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            kruskal_step(&k_state);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        draw_weighted_edges(&all_edges, current_nodes, NODE_RADIUS, LIGHTGRAY, GRAY);

        draw_weighted_edges(&k_state.mst_edges, current_nodes, NODE_RADIUS, BLACK, MAROON);

        if (k_state.current_edge != NULL && !k_state.finished)
        {
            EdgeList temp_list = init_edge_list();
            add_edge(&temp_list, k_state.current_edge->start, k_state.current_edge->end, k_state.current_edge->weight);

            draw_weighted_edges(&temp_list, current_nodes, NODE_RADIUS, ORANGE, RED);
            free_edge_list(&temp_list);
        }

        float hueStep = 360.0f / GRAPH_N;
        int fontSize = (int)(NODE_RADIUS * 1.25f);

        for (int i = 0; i < GRAPH_N; i++)
        {
            Color nodeColor = ColorFromHSV(i * hueStep, 0.7f, 0.9f);
            DrawCircleV(current_nodes[i], NODE_RADIUS, nodeColor);

            char text[10];
            snprintf(text, sizeof(text), "%d", i + 1);
            int textWidth = MeasureText(text, fontSize);
            int textX = (int)current_nodes[i].x - (textWidth / 2);
            int textY = (int)current_nodes[i].y - (fontSize / 2);
            DrawText(text, textX, textY, fontSize, RAYWHITE);
        }

        const char *title = TextFormat("Press 'SPACE' to step | MST Total Weight: %d", k_state.total_weight);
        DrawText(title, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, DARKGRAY);

        if (k_state.finished)
        {
            DrawText("ALGORITHM FINISHED!", TEXT_SIZE, TEXT_SIZE * 2 + 10, TEXT_SIZE, MAROON);
        }

        EndDrawing();
    }

    CloseWindow();

    free_imatrix(&A_dir);
    free_imatrix(&A);
    free_dmatrix(&B);
    free_imatrix(&C);
    free_imatrix(&D);
    free_imatrix(&H);
    free_imatrix(&T);
    free_imatrix(&W);

    free_edge_list(&all_edges);
    free_edge_list(&k_state.mst_edges);

    return 0;
}