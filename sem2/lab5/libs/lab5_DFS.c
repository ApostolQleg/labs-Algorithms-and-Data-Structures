#include "lab5_DFS.h"

static int get_next_start_node(TraversalState *state, const IMatrix *matrix)
{
    for (int i = 0; i < state->n; i++)
    {
        if (state->visited[i] == 0)
        {
            for (int j = 0; j < state->n; j++)
            {
                if (matrix->data[i][j] > 0)
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

void step_DFS(TraversalState *state, const IMatrix *matrix)
{
    if (state->is_finished)
        return;

    if (state->tail > 0)
    {
        int current = state->queue_stack[state->tail - 1];
        state->current_node = current;

        bool found_neighbor = false;

        for (int i = 0; i < state->n; i++)
        {
            if (matrix->data[current][i] > 0 && state->visited[i] == 0)
            {
                state->visited[i] = 1;
                state->tree_edges.data[current][i] = 1;

                state->queue_stack[state->tail++] = i;
                found_neighbor = true;
                break;
            }
        }

        if (!found_neighbor)
        {
            state->tail--;
        }
    }

    if (state->tail == 0)
    {
        int next_start = get_next_start_node(state, matrix);
        if (next_start != -1)
        {
            state->visited[next_start] = 1;
            state->queue_stack[state->tail++] = next_start;

            state->current_node = next_start;
        }
        else
        {
            state->is_finished = true;
            state->current_node = -1;
        }
    }
}