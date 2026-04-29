#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph_lib.h"

#define ARROW_ANGLE (PI / 6.0f)
#define LOOP_DIST_FACTOR 1.5f
#define LOOP_RADIUS_FACTOR 1.25f
#define LOOP_ARROW_OFFSET (PI / 4.0f)

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

double randm()
{
    return ((double)rand() / RAND_MAX) * 2.0;
}

int mulmr(double value, double k)
{
    return (value * k >= 1.0) ? 1 : 0;
}

void generate_directed_matrix(int **A_dir, int n, double k)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double T = randm();
            A_dir[i][j] = mulmr(T, k);
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
            else
            {
                A_undir[i][j] = 0;
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

void draw_arrow(Vector2 target, float angle, float arrow_length)
{
    Vector2 v1 = {target.x, target.y};
    Vector2 v2 = {
        target.x - arrow_length * cosf(angle - ARROW_ANGLE),
        target.y - arrow_length * sinf(angle - ARROW_ANGLE)};
    Vector2 v3 = {
        target.x - arrow_length * cosf(angle + ARROW_ANGLE),
        target.y - arrow_length * sinf(angle + ARROW_ANGLE)};

    DrawTriangle(v1, v2, v3, GRAY);
    DrawTriangle(v1, v3, v2, GRAY);
}

void draw_self_loop(Vector2 node, Vector2 center, float r_node, bool show_arrow)
{
    float dx = node.x - center.x;
    float dy = node.y - center.y;
    float length = sqrtf(dx * dx + dy * dy);

    Vector2 loop_center = {
        node.x + (dx / length) * (r_node * LOOP_DIST_FACTOR),
        node.y + (dy / length) * (r_node * LOOP_DIST_FACTOR)};

    float r_loop = r_node * LOOP_RADIUS_FACTOR;

    DrawCircleLines(loop_center.x, loop_center.y, r_loop, GRAY);

    if (show_arrow)
    {
        float arrow_length = r_node * 0.375f;

        float center_based_alpha = atan2f(node.y - loop_center.y, node.x - loop_center.x);

        float alpha1 = center_based_alpha - LOOP_ARROW_OFFSET;
        Vector2 target1 = {
            loop_center.x + r_loop * cosf(alpha1),
            loop_center.y + r_loop * sinf(alpha1)};
        float angle1 = alpha1 + (PI / 2.0f);
        draw_arrow(target1, angle1, arrow_length);

        float alpha2 = center_based_alpha + LOOP_ARROW_OFFSET;
        Vector2 target2 = {
            loop_center.x + r_loop * cosf(alpha2),
            loop_center.y + r_loop * sinf(alpha2)};
        float angle2 = alpha2 - (PI / 2.0f);
        draw_arrow(target2, angle2, arrow_length);
    }
}

void draw_graph(int **matrix, Vector2 *nodes, int n, float r_node, Vector2 center, bool is_directed)
{
    float arrow_length = r_node * 0.375f;
    float node_padding = r_node * 0.05f;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 1)
            {
                if (!is_directed && j < i)
                    continue;

                if (i != j)
                {
                    DrawLineV(nodes[i], nodes[j], GRAY);

                    if (is_directed)
                    {
                        float angle = atan2f(nodes[j].y - nodes[i].y, nodes[j].x - nodes[i].x);
                        Vector2 target = {
                            nodes[j].x - (r_node + node_padding) * cosf(angle),
                            nodes[j].y - (r_node + node_padding) * sinf(angle)};
                        draw_arrow(target, angle, arrow_length);
                    }
                }
                else
                {
                    draw_self_loop(nodes[i], center, r_node, is_directed);
                }
            }
        }
    }

    int fontSize = (int)(r_node * 1.25f);

    for (int i = 0; i < n; i++)
    {
        DrawCircleV(nodes[i], r_node, LIGHTGRAY);
        DrawCircleLines(nodes[i].x, nodes[i].y, r_node, DARKGRAY);

        char text[10];
        snprintf(text, sizeof(text), "%d", i);

        int textWidth = MeasureText(text, fontSize);

        int textX = (int)nodes[i].x - (textWidth / 2);
        int textY = (int)nodes[i].y - (fontSize / 2);

        DrawText(text, textX, textY, fontSize, BLACK);
    }
}