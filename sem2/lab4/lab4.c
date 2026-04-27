#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "raylib.h"

#define N1 5
#define N2 4
#define N3 0
#define N4 4
#define SEED ((N1 * 1000) + (N2 * 100) + (N3 * 10) + N4)
#define K (1 - (N3 * 0.02) - (N4 * 0.005) - 0.25)
#define N (10 + N3)

int **create_matrix(int n)
{
    int **matrix = (int **)calloc(n, sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        matrix[i] = (int *)calloc(n, sizeof(int));
    }
    return matrix;
}

void destroy_matrix(int **matrix, int n)
{
    if (matrix == NULL)
        return;

    for (int i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

double random()
{
    return ((double)rand() / RAND_MAX) * 2.0;
}

int mulmr(double value, double k)
{
    double result = value * k;
    if (result >= 1.0)
    {
        return 1;
    }
    return 0;
}

void generate_directed_matrix(int **matrix, int n, double k)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double T = random();
            matrix[i][j] = mulmr(T, k);
        }
    }
}

void generate_undirected_matrix(int **A_dir, int **A_undir, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (A_dir[i][j] == 1)
            {
                A_undir[i][j] = A_undir[j][i] = 1;
            }
        }
    }
}

void print_matrix(int **matrix, int n, const char *title)
{
    printf("\n%s:\n", title);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void draw_arrow(Vector2 start, Vector2 end, float r_node)
{
    float angle = atan2f(end.y - start.y, end.x - start.x);

    float target_x = end.x - (r_node + 2) * cosf(angle);
    float target_y = end.y - (r_node + 2) * sinf(angle);

    float arrow_len = 15.0f;
    float arrow_angle = PI / 6.0f;

    Vector2 v1 = {target_x, target_y};
    Vector2 v2 = {target_x - arrow_len * cosf(angle - arrow_angle), target_y - arrow_len * sinf(angle - arrow_angle)};
    Vector2 v3 = {target_x - arrow_len * cosf(angle + arrow_angle), target_y - arrow_len * sinf(angle + arrow_angle)};

    DrawTriangle(v1, v2, v3, GRAY);
    DrawTriangle(v1, v3, v2, GRAY);
}

int main()
{
    int **A_dir = create_matrix(N);
    int **A_undir = create_matrix(N);

    srand(SEED);
    generate_directed_matrix(A_dir, N, K);
    generate_undirected_matrix(A_dir, A_undir, N);

    print_matrix(A_dir, N, "Directed Graph Matrix");
    print_matrix(A_undir, N, "Undirected Graph Matrix");

    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Lab 3 - Raylib Graph Representation");
    SetTargetFPS(60);

    bool show_directed = true;
    float R = 400.0f;
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

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (current_matrix[i][j] == 1)
                {
                    DrawLineV(nodes[i], nodes[j], GRAY);

                    if (show_directed && i != j)
                    {
                        draw_arrow(nodes[i], nodes[j], r_node);
                    }
                }
            }
        }

        for (int i = 0; i < N; i++)
        {
            DrawCircleV(nodes[i], r_node, LIGHTGRAY);
            DrawCircleLines(nodes[i].x, nodes[i].y, r_node, DARKGRAY);

            char text[3];
            sprintf(text, "%d", i);

            int fontSize = 50;
            int textWidth = MeasureText(text, fontSize);

            int textX = nodes[i].x - (textWidth / 2);
            int textY = nodes[i].y - (fontSize / 2);

            DrawText(text, textX, textY, fontSize, BLACK);
        }

        const char *title = show_directed ? "Directed Graph (Press 'SPACE' to switch)" : "Undirected Graph (Press 'SPACE' to switch)";
        DrawText(title, 20, 20, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    destroy_matrix(A_dir, N);
    destroy_matrix(A_undir, N);

    return 0;
}