#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "graph_lib.h"
#include "n1_n2_n3_n4.h"
#include "lab5_BFS.h"
#include "lab5_DFS.h"
#include "lab5_state.h"

#define K (1 - (N3 * 0.01) - (N4 * 0.005) - 0.15)
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

    TraversalState bfs_states[2];
    TraversalState dfs_states[2];
    TraversalHistory bfs_histories[2];
    TraversalHistory dfs_histories[2];

    for (int i = 0; i < 2; i++)
    {
        init_traversal_state(&bfs_states[i], N);
        init_traversal_state(&dfs_states[i], N);
        init_history(&bfs_histories[i]);
        init_history(&dfs_histories[i]);
    }

    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 5 - Graph Traversal (BFS and DFS)");
    SetTargetFPS(60);

    int **show_matrices[2] = {A_dir, A_undir};
    int show_sizes[2] = {N, N};
    bool show_is_dir[2] = {true, false};
    const char *show_titles[2] = {"Directed", "Undirected"};

    const char *trav_titles[2] = {"BFS", "DFS"};

    int curr = 0;
    int curr_trav = 0;
    Vector2 center = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            curr = (curr + 1) % 2;
        }

        TraversalState *current_bfs = &bfs_states[curr];
        TraversalState *current_dfs = &dfs_states[curr];
        TraversalHistory *current_bfs_hist = &bfs_histories[curr];
        TraversalHistory *current_dfs_hist = &dfs_histories[curr];

        int current_n = show_sizes[curr];
        int **current_matrix = show_matrices[curr];
        bool is_dir = show_is_dir[curr];

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT))
        {
            curr_trav = (curr_trav + 1) % 2;
        }

        if (IsKeyPressed(KEY_UP))
        {
            if (curr_trav == 0 && !current_bfs->is_finished)
            {
                save_state(current_bfs_hist, current_bfs);
                step_BFS(current_bfs, current_matrix);
            }
            else if (curr_trav == 1 && !current_dfs->is_finished)
            {
                save_state(current_dfs_hist, current_dfs);
                step_DFS(current_dfs, current_matrix);
            }
        }

        if (IsKeyPressed(KEY_DOWN))
        {
            if (curr_trav == 0)
            {
                undo_state(current_bfs_hist, current_bfs);
            }
            else if (curr_trav == 1)
            {
                undo_state(current_dfs_hist, current_dfs);
            }
        }

        Vector2 current_nodes[current_n];
        for (int i = 0; i < current_n; i++)
        {
            float angle = 2.0f * PI * i / current_n;
            current_nodes[i].x = center.x + GRAPH_RADIUS * cosf(angle);
            current_nodes[i].y = center.y + GRAPH_RADIUS * sinf(angle);
        }

        TraversalState *current_state = (curr_trav == 0) ? current_bfs : current_dfs;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        draw_graph_traversal(current_matrix, current_nodes, current_n, NODE_RADIUS, center, is_dir, current_state->visited, current_state->tree_edges);

        const char *title = TextFormat("Showing: %s [Press 'SPACE' to switch]", show_titles[curr]);
        const char *trav_mode = TextFormat("Current traversal mode: %s [Press 'RIGHT' or 'LEFT' to switch] [Press 'UP' to start traversal]", trav_titles[curr_trav]);

        DrawText(title, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, DARKGRAY);
        DrawText(trav_mode, TEXT_SIZE, TEXT_SIZE * 3, TEXT_SIZE / 1.5, GRAY);

        EndDrawing();
    }

    CloseWindow();

    destroy_matrix(A_dir, N);
    destroy_matrix(A_undir, N);

    for (int i = 0; i < 2; i++)
    {
        free_traversal_state(&bfs_states[i]);
        free_traversal_state(&dfs_states[i]);
        free_history(&bfs_histories[i]);
        free_history(&dfs_histories[i]);
    }

    return 0;
}