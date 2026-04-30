#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lab4_1.h"

void calc_degrees_dir(const IMatrix *matrix, int *out_degrees, int *in_degrees)
{
    int n = matrix->N;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix->data[i][j] == 1)
            {
                out_degrees[i]++;
                in_degrees[j]++;
            }
        }
    }
}

void calc_degrees_undir(const IMatrix *matrix, int *undir_degrees)
{
    int n = matrix->N;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix->data[i][j] == 1)
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

void draw_degrees(const IMatrix *matrix, Vector2 *nodes, float r_node, Vector2 center, bool is_dir)
{
    int n = matrix->N;
    int fontSize = (int)(r_node * 0.75f);
    float offset = r_node * 2.75f;

    Font defaultFont = GetFontDefault();

    for (int i = 0; i < n; i++)
    {
        int in_deg = 0, out_deg = 0, deg = 0;

        for (int j = 0; j < n; j++)
        {
            if (matrix->data[i][j] == 1)
            {
                out_deg++;
                deg += (i == j) ? 2 : 1;
            }
            if (matrix->data[j][i] == 1)
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
        printf("\n %s graph doesn't have pendant nodes.\n", graph_name);
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
        printf("\n %s graph doesn't have isolated nodes.\n", graph_name);
    }
}