#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph_lib.h"

#define ARROW_ANGLE (PI / 7.0f)
#define LOOP_DIST_FACTOR 1.5f
#define LOOP_RADIUS_FACTOR 1.25f
#define LOOP_ARROW_OFFSET (PI / 4.5f)

IMatrix init_imatrix(int n)
{
    IMatrix matrix = {0};
    matrix.N = n;
    matrix.data = (int **)calloc(n, sizeof(int *));

    for (int i = 0; i < n; i++)
    {
        matrix.data[i] = (int *)calloc(n, sizeof(int));
    }

    return matrix;
}

void free_imatrix(IMatrix *matrix)
{
    if (matrix == NULL || matrix->data == NULL)
        return;

    for (int i = 0; i < matrix->N; i++)
    {
        free(matrix->data[i]);
    }

    free(matrix->data);
    matrix->data = NULL;
    matrix->N = 0;
}

double randm()
{
    return ((double)rand() / RAND_MAX) * 2.0;
}

int mulmr(double value, double k)
{
    return (value * k >= 1.0) ? 1 : 0;
}

void seed_directed_matrix(IMatrix *matrix_dir, double k)
{
    int n = matrix_dir->N;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double T = randm();
            matrix_dir->data[i][j] = mulmr(T, k);
        }
    }
}

void seed_undirected_matrix(const IMatrix *matrix_dir, IMatrix *matrix_undir)
{
    int n = matrix_dir->N;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix_dir->data[i][j] == 1)
            {
                matrix_undir->data[i][j] = matrix_undir->data[j][i] = 1;
            }
        }
    }
}

void print_matrix(const IMatrix *matrix, const char *title)
{
    int n = matrix->N;

    printf("\n%s:\n", title);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%3d ", matrix->data[i][j]);
        }
        printf("\n\n");
    }
}

void draw_arrow(Vector2 target, float angle, float arrow_length, Color color)
{
    Vector2 v1 = {target.x, target.y};
    Vector2 v2 = {
        target.x - arrow_length * cosf(angle - ARROW_ANGLE),
        target.y - arrow_length * sinf(angle - ARROW_ANGLE)};
    Vector2 v3 = {
        target.x - arrow_length * cosf(angle + ARROW_ANGLE),
        target.y - arrow_length * sinf(angle + ARROW_ANGLE)};

    DrawTriangle(v1, v2, v3, color);
    DrawTriangle(v1, v3, v2, color);
}

void draw_self_loop(Vector2 node, Vector2 center, float r_node, bool show_arrow, Color color)
{
    float dx = node.x - center.x;
    float dy = node.y - center.y;
    float length = sqrtf(dx * dx + dy * dy);

    Vector2 loop_center = {
        node.x + (dx / length) * (r_node * LOOP_DIST_FACTOR),
        node.y + (dy / length) * (r_node * LOOP_DIST_FACTOR)};

    float r_loop = r_node * LOOP_RADIUS_FACTOR;

    DrawCircleLines(loop_center.x, loop_center.y, r_loop, color);

    if (show_arrow)
    {
        float arrow_length = r_node * 0.5f;

        float center_based_alpha = atan2f(node.y - loop_center.y, node.x - loop_center.x);

        float alpha1 = center_based_alpha - LOOP_ARROW_OFFSET;
        Vector2 target1 = {
            loop_center.x + r_loop * cosf(alpha1),
            loop_center.y + r_loop * sinf(alpha1)};
        float angle1 = alpha1 + (PI / 2.2f);
        draw_arrow(target1, angle1, arrow_length, color);

        float alpha2 = center_based_alpha + LOOP_ARROW_OFFSET;
        Vector2 target2 = {
            loop_center.x + r_loop * cosf(alpha2),
            loop_center.y + r_loop * sinf(alpha2)};
        float angle2 = alpha2 - (PI / 2.2f);
        draw_arrow(target2, angle2, arrow_length, color);
    }
}
void draw_graph(const IMatrix *matrix, Vector2 *nodes, float r_node, Vector2 center, bool is_directed)
{
    draw_graph_traversal(matrix, nodes, r_node, center, is_directed, NULL, NULL);
}

void draw_graph_traversal(const IMatrix *matrix, Vector2 *nodes, float r_node, Vector2 center, bool is_directed, int *visited, const IMatrix *tree_edges)
{
    int n = matrix->N;
    float arrow_length = r_node * 0.5f;
    float node_padding = r_node * 0.05f;
    float curvature = 15.0f;
    float hueStep = 360.0f / n;

    for (int pass = 0; pass < 2; pass++)
    {
        for (int i = 0; i < n; i++)
        {
            Color baseNodeColor = ColorFromHSV(i * hueStep, 0.7f, 0.9f);

            for (int j = 0; j < n; j++)
            {
                if (matrix->data[i][j] == 1)
                {
                    if (!is_directed && j < i)
                        continue;

                    bool is_tree_edge = false;
                    if (tree_edges != NULL)
                    {
                        is_tree_edge = (tree_edges->data[i][j] == 1);
                        if (!is_directed && tree_edges->data[j][i] == 1)
                        {
                            is_tree_edge = true;
                        }
                    }

                    if (pass == 0 && is_tree_edge)
                        continue;
                    if (pass == 1 && !is_tree_edge)
                        continue;

                    Color edgeColor = baseNodeColor;
                    float line_thickness = 2.0f;

                    if (tree_edges != NULL)
                    {
                        edgeColor = is_tree_edge ? BLACK : LIGHTGRAY;
                        line_thickness = is_tree_edge ? 4.0f : 2.0f;
                    }

                    if (i != j)
                    {
                        Vector2 start = nodes[i];
                        Vector2 end = nodes[j];

                        Vector2 mid = {(start.x + end.x) / 2, (start.y + end.y) / 2};
                        float dx = end.x - start.x;
                        float dy = end.y - start.y;
                        float dist = sqrtf(dx * dx + dy * dy);

                        Vector2 normal = {-dy / dist, dx / dist};

                        float current_bend = is_directed ? curvature : 0.0f;
                        Vector2 control = {
                            mid.x + normal.x * current_bend,
                            mid.y + normal.y * current_bend};

                        DrawSplineSegmentBezierQuadratic(start, control, end, line_thickness, edgeColor);

                        if (is_directed)
                        {
                            float t = 1.0f;
                            Vector2 target = end;

                            while (t > 0.0f)
                            {
                                float q0 = (1.0f - t) * (1.0f - t);
                                float q1 = 2.0f * t * (1.0f - t);
                                float q2 = t * t;

                                target.x = q0 * start.x + q1 * control.x + q2 * end.x;
                                target.y = q0 * start.y + q1 * control.y + q2 * end.y;

                                float dx_to_center = target.x - end.x;
                                float dy_to_center = target.y - end.y;
                                float distance = sqrtf(dx_to_center * dx_to_center + dy_to_center * dy_to_center);

                                if (distance >= (r_node + node_padding))
                                {
                                    break;
                                }
                                t -= 0.01f;
                            }

                            Vector2 tangent = {
                                2.0f * (1.0f - t) * (control.x - start.x) + 2.0f * t * (end.x - control.x),
                                2.0f * (1.0f - t) * (control.y - start.y) + 2.0f * t * (end.y - control.y)};
                            float angle = atan2f(tangent.y, tangent.x);

                            draw_arrow(target, angle, arrow_length, edgeColor);
                        }
                    }
                    else
                    {
                        draw_self_loop(nodes[i], center, r_node, is_directed, edgeColor);
                    }
                }
            }
        }
    }

    int fontSize = (int)(r_node * 1.25f);

    for (int i = 0; i < n; i++)
    {
        Color nodeColor = ColorFromHSV(i * hueStep, 0.7f, 0.9f);

        if (visited != NULL && visited[i] == 1)
        {
            nodeColor = BLACK;
        }

        DrawCircleV(nodes[i], r_node, nodeColor);

        char text[10];
        snprintf(text, sizeof(text), "%d", i + 1);

        int textWidth = MeasureText(text, fontSize);

        int textX = (int)nodes[i].x - (textWidth / 2);
        int textY = (int)nodes[i].y - (fontSize / 2);

        DrawText(text, textX, textY, fontSize, RAYWHITE);
    }
}

EdgeList init_edge_list()
{
    EdgeList list = {NULL, 0};
    return list;
}

void add_edge(EdgeList *list, int start, int end, int weight)
{
    Edge *new_edge = (Edge *)malloc(sizeof(Edge));
    if (new_edge == NULL)
        return;

    new_edge->start = start;
    new_edge->end = end;
    new_edge->weight = weight;

    new_edge->next = list->head;
    list->head = new_edge;

    list->size++;
}

void free_edge_list(EdgeList *list)
{
    Edge *current = list->head;
    while (current != NULL)
    {
        Edge *temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
    list->size = 0;
}

void print_edge_list(const EdgeList *list, const char *title)
{
    printf("\n%s:\n", title);

    if (list == NULL || list->head == NULL)
    {
        printf("Edge list is empty.\n\n");
        return;
    }

    Edge *current = list->head;
    int count = 1;

    while (current != NULL)
    {
        printf("%3d. Edge (%2d, %2d) | Weight: %d\n",
               count,
               current->start + 1,
               current->end + 1,
               current->weight);

        current = current->next;
        count++;
    }
    printf("There is %d edges\n\n", list->size);
}

void draw_weighted_edges(EdgeList *list, Vector2 *nodes, float r_node, Color edgeColor, Color textColor)
{
    if (list == NULL || list->head == NULL)
        return;

    Edge *current = list->head;
    int fontSize = (int)(r_node * 0.7f);

    while (current != NULL)
    {
        Vector2 start = nodes[current->start];
        Vector2 end = nodes[current->end];

        DrawLineEx(start, end, 3.0f, edgeColor);

        Vector2 mid = {(start.x + end.x) / 2.0f, (start.y + end.y) / 2.0f};

        char weight_text[10];
        snprintf(weight_text, sizeof(weight_text), "%d", current->weight);

        int textWidth = MeasureText(weight_text, fontSize);

        DrawRectangle(mid.x - textWidth / 2 - 2, mid.y - fontSize / 2 - 2, textWidth + 4, fontSize + 4, Fade(RAYWHITE, 0.8f));
        DrawText(weight_text, mid.x - textWidth / 2, mid.y - fontSize / 2, fontSize, textColor);

        current = current->next;
    }
}
