#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "graph_lib.h"
#include "n1_n2_n3_n4.h"

#define K1 (1 - (N3 * 0.01) - (N4 * 0.01) - 0.3)
#define K2 (1 - (N3 * 0.005) - (N4 * 0.005) - 0.27)
#define N (10 + N3)

void calc_degrees_dir(int **matrix, int n, int *out_degrees, int *in_degrees);
void calc_degrees_undir(int **matrix, int n, int *undir_degrees);
void print_degrees(int *degrees, int n, char *message);
void draw_degrees(int **matrix, Vector2 *nodes, int n, float r_node, Vector2 center, bool is_dir);
int calc_regularity(int *degrees, int n);
void print_regularity(int r_degree, char *graph_name);
int calc_pendant(int *degrees, int n, int *p_deg);
void print_pendant(int *p_deg, int n, int count, char *graph_name);
int calc_isolated(int *degrees, int n, int *i_deg);
void print_isolated(int *i_deg, int n, int count, char *graph_name);

int main()
{
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 1200;
    const int TEXT_SIZE = 30;
    const float GRAPH_RADIUS = 250.0f;
    const float NODE_RADIUS = 30.0f;

    srand(SEED);

    int **A1_dir = create_matrix(N);
    int **A1_undir = create_matrix(N);
    int **A2_dir = create_matrix(N);
    int **A2_undir = create_matrix(N);

    generate_directed_matrix(A1_dir, N, K1);
    generate_undirected_matrix(A1_dir, A1_undir, N);

    generate_directed_matrix(A2_dir, N, K2);
    generate_undirected_matrix(A2_dir, A2_undir, N);

    print_matrix(A1_dir, N, "Directed Graph Matrix (K1)");
    print_matrix(A1_undir, N, "Undirected Graph Matrix (K1)");

    print_matrix(A2_dir, N, "Directed Graph Matrix (K2)");
    print_matrix(A2_undir, N, "Undirected Graph Matrix (K2)");

    int out_deg[N] = {0};
    int in_deg[N] = {0};
    int undir_deg[N] = {0};
    calc_degrees_dir(A1_dir, N, out_deg, in_deg);
    calc_degrees_undir(A1_undir, N, undir_deg);
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
        draw_degrees(show[curr], nodes, N, NODE_RADIUS, center, is_dir);

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

void calc_degrees_dir(int **matrix, int n, int *out_degrees, int *in_degrees)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 1)
            {
                out_degrees[i]++;
                in_degrees[j]++;
            }
        }
    }
}

void calc_degrees_undir(int **matrix, int n, int *undir_degrees)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 1)
            {
                undir_degrees[i] += (i == j) ? 2 : 1;
            }
        }
    }
}

void print_degrees(int *degrees, int n, char *message)
{
    printf("\n%s\n", message);
    for (int i = 0; i < n; i++)
    {
        printf("%3d: %3d\n", i + 1, degrees[i]);
    }
}

void draw_degrees(int **matrix, Vector2 *nodes, int n, float r_node, Vector2 center, bool is_dir)
{
    int fontSize = (int)(r_node * 0.75f);
    float offset = r_node * 2.75f;

    Font defaultFont = GetFontDefault();

    for (int i = 0; i < n; i++)
    {
        int in_deg = 0, out_deg = 0, deg = 0;

        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 1)
            {
                out_deg++;
                deg += (i == j) ? 2 : 1;
            }
            if (matrix[j][i] == 1)
                in_deg++;
        }

        float dx = nodes[i].x - center.x;
        float dy = nodes[i].y - center.y;
        float dist = sqrtf(dx * dx + dy * dy);

        Vector2 textPos = {
            nodes[i].x + (dx / dist) * (r_node + offset),
            nodes[i].y + (dy / dist) * (r_node + offset)};

        float angleRadians = atan2f(dy, dx);
        float rotation = angleRadians * RAD2DEG;

        if (cosf(angleRadians) < 0)
        {
            rotation += 180.0f;
        }

        const char *text;
        if (is_dir)
        {
            text = TextFormat("I : %d\nO : %d", in_deg, out_deg);
        }
        else
        {
            text = TextFormat("D : %d", deg);
        }

        Vector2 textSize = MeasureTextEx(defaultFont, text, fontSize, 1.0f);

        Vector2 origin = {textSize.x / 2.0f, textSize.y / 2.0f};

        DrawTextPro(defaultFont, text, textPos, origin, rotation, fontSize, 1.0f, DARKGRAY);
    }
}

int calc_regularity(int *degrees, int n)
{
    int deg = degrees[0];
    for (int i = 0; i < n; i++)
    {
        if (degrees[i] != deg)
        {
            return -1;
        }
    }
    return deg;
}

void print_regularity(int r_degree, char *graph_name)
{
    if (r_degree >= 0)
    {
        printf("\n %s graph is regular and it's regularity degree is %d.\n", graph_name, r_degree);
    }
    else
    {
        printf("\n %s graph is not regular.\n", graph_name);
    }
}

int calc_pendant(int *degrees, int n, int *p_deg)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (degrees[i] == 1)
        {
            p_deg[i] = 1;
            count++;
        }
    }
    return count;
}

void print_pendant(int *p_deg, int n, int count, char *graph_name)
{
    if (count > 0)
    {
        printf("\n %s graph has pendant nodes:\n", graph_name);
        for (int i = 0; i < n; i++)
        {
            if (p_deg[i] == 1)
            {
                printf("   %d", i + 1);
            }
        }
    }
    else
    {
        printf("\n %s graph doesn't have pendant nodes:\n", graph_name);
    }
}

int calc_isolated(int *degrees, int n, int *i_deg)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (degrees[i] == 0)
        {
            i_deg[i] = 1;
            count++;
        }
    }
    return count;
}

void print_isolated(int *i_deg, int n, int count, char *graph_name)
{
    if (count > 0)
    {
        printf("\n %s graph has isolated nodes:\n", graph_name);
        for (int i = 0; i < n; i++)
        {
            if (i_deg[i] == 1)
            {
                printf("   %d", i + 1);
            }
        }
    }
    else
    {
        printf("\n %s graph doesn't have isolated nodes:\n", graph_name);
    }
}