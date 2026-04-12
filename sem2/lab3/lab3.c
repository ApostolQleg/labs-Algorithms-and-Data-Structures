#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <windows.h>

#define PI 3.14159265358979323846
#define SEED 5400
#define K 0.75
#define N 10

int **global_A_dir = NULL;
int **global_A_undir = NULL;
bool show_directed = true;

int **create_matrix(int n)
{
    int **matrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        matrix[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = 0;
        }
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

int calculate_edge(double value, double k)
{
    double result = value * k;
    if (result >= 1.0)
    {
        return 1;
    }
    return 0;
}

void generate_directed_matrix(int **matrix, int n, int seed, double k)
{
    srand(seed);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double T = random();
            matrix[i][j] = calculate_edge(T, k);
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
                A_undir[i][j] = 1;
                A_undir[j][i] = 1;
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

void draw_graph(HDC hdc, RECT client_rect)
{
    int cx = (client_rect.right - client_rect.left) / 2;
    int cy = (client_rect.bottom - client_rect.top) / 2;
    int R = 200;
    int r_node = 20;

    int **current_matrix = show_directed ? global_A_dir : global_A_undir;

    POINT nodes[N];

    for (int i = 0; i < N; i++)
    {
        double angle = 2.0 * PI * i / N;
        nodes[i].x = cx + (int)(R * cos(angle));
        nodes[i].y = cy + (int)(R * sin(angle));
    }

    HPEN edge_pen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    HPEN old_pen = (HPEN)SelectObject(hdc, edge_pen);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (current_matrix[i][j] == 1)
            {
                MoveToEx(hdc, nodes[i].x, nodes[i].y, NULL);
                LineTo(hdc, nodes[j].x, nodes[j].y);

                if (show_directed && i != j)
                {
                    double angle = atan2((double)(nodes[j].y - nodes[i].y), (double)(nodes[j].x - nodes[i].x));

                    int target_x = nodes[j].x - (int)((r_node + 2) * cos(angle));
                    int target_y = nodes[j].y - (int)((r_node + 2) * sin(angle));

                    int arrow_len = 12;
                    double arrow_angle = PI / 6;

                    POINT pts[3];
                    pts[0].x = target_x;
                    pts[0].y = target_y;
                    pts[1].x = target_x - (int)(arrow_len * cos(angle - arrow_angle));
                    pts[1].y = target_y - (int)(arrow_len * sin(angle - arrow_angle));
                    pts[2].x = target_x - (int)(arrow_len * cos(angle + arrow_angle));
                    pts[2].y = target_y - (int)(arrow_len * sin(angle + arrow_angle));

                    HBRUSH arrow_brush = CreateSolidBrush(RGB(150, 150, 150));
                    HBRUSH old_brush = (HBRUSH)SelectObject(hdc, arrow_brush);
                    Polygon(hdc, pts, 3);

                    SelectObject(hdc, old_brush);
                    DeleteObject(arrow_brush);
                }
            }
        }
    }
    SelectObject(hdc, old_pen);
    DeleteObject(edge_pen);

    HPEN node_pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 200));
    HBRUSH node_brush = CreateSolidBrush(RGB(240, 240, 255));
    SelectObject(hdc, node_pen);
    SelectObject(hdc, node_brush);
    SetBkMode(hdc, TRANSPARENT);

    for (int i = 0; i < N; i++)
    {
        Ellipse(hdc, nodes[i].x - r_node, nodes[i].y - r_node, nodes[i].x + r_node, nodes[i].y + r_node);

        char text[3];
        sprintf(text, "%d", i);
        TextOutA(hdc, nodes[i].x - 4, nodes[i].y - 8, text, strlen(text));
    }

    SelectObject(hdc, GetStockObject(BLACK_PEN));
    SelectObject(hdc, GetStockObject(WHITE_BRUSH));
    DeleteObject(node_pen);
    DeleteObject(node_brush);

    const char *title = show_directed ? "Directed Graph (Press 'SPACE' to switch)" : "Undirected Graph (Press 'SPACE' to switch)";
    TextOutA(hdc, 20, 20, title, strlen(title));
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        draw_graph(hdc, rect);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_KEYDOWN:
    {
        if (wParam == VK_SPACE)
        {
            show_directed = !show_directed;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main()
{
    global_A_dir = create_matrix(N);
    global_A_undir = create_matrix(N);

    generate_directed_matrix(global_A_dir, N, SEED, K);
    generate_undirected_matrix(global_A_dir, global_A_undir, N);

    printf("Graph properties:\n");
    printf("Vertices (n) = %d\n", N);
    printf("Seed = %d\n", SEED);
    printf("k = %.2f\n", K);

    print_matrix(global_A_dir, N, "Directed Graph Matrix");
    print_matrix(global_A_undir, N, "Undirected Graph Matrix");

    const char CLASS_NAME[] = "GraphWindow";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Lab 3 - Graph Representation",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, wc.hInstance, NULL);

    if (hwnd == NULL)
    {
        printf("Error: Unable to create graphical window.\n");
        return 0;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);

    printf("\nGraphical window opened.\n");
    printf("\nClose the window to exit the program.\n");

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    destroy_matrix(global_A_dir, N);
    destroy_matrix(global_A_undir, N);

    return 0;
}