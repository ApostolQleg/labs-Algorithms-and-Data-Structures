#include "lab5_BFS.h"

static int get_next_start_node(TraversalState *state, int **matrix)
{
    for (int i = 0; i < state->n; i++)
    {
        if (state->visited[i] == 0)
        {
            for (int j = 0; j < state->n; j++)
            {
                if (matrix[i][j] > 0)
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

void step_BFS(TraversalState *state, int **matrix)
{
    if (state->is_finished)
        return;

    if (state->head < state->tail)
    {
        int current = state->queue_stack[state->head];
        state->current_node = current;

        for (int i = 0; i < state->n; i++)
        {
            if (matrix[current][i] > 0 && state->visited[i] == 0)
            {
                state->visited[i] = 1;
                state->tree_edges[current][i] = 1;
                state->queue_stack[state->tail++] = i;
            }
        }

        state->head++;
    }

    if (state->head == state->tail)
    {
        int next_start = get_next_start_node(state, matrix);
        if (next_start != -1)
        {
            state->visited[next_start] = 1;
            state->queue_stack[state->tail++] = next_start;
        }
        else
        {
            state->is_finished = true;
            state->current_node = -1;
        }
    }
}