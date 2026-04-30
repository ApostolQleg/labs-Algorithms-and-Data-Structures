#include <string.h>
#include "lab5_state.h"

void init_traversal_state(TraversalState *state, int n)
{
    state->n = n;

    state->visited = (int *)calloc(n, sizeof(int));
    state->queue_stack = (int *)calloc(n, sizeof(int));

    state->tree_edges = init_imatrix(n);

    state->head = 0;
    state->tail = 0;
    state->current_node = -1;
    state->is_finished = false;
}

void free_traversal_state(TraversalState *state)
{
    if (state->visited != NULL)
    {
        free(state->visited);
    }
    if (state->queue_stack != NULL)
    {
        free(state->queue_stack);
    }
    if (state->tree_edges.data != NULL)
    {
        free_imatrix(&state->tree_edges);
    }
}

void copy_traversal_state(TraversalState *dest, const TraversalState *src)
{
    if (dest->visited == NULL)
    {
        init_traversal_state(dest, src->n);
    }

    dest->n = src->n;
    dest->head = src->head;
    dest->tail = src->tail;
    dest->current_node = src->current_node;
    dest->is_finished = src->is_finished;

    memcpy(dest->visited, src->visited, src->n * sizeof(int));
    memcpy(dest->queue_stack, src->queue_stack, src->n * sizeof(int));

    for (int i = 0; i < src->n; i++)
    {
        memcpy(dest->tree_edges.data[i], src->tree_edges.data[i], src->n * sizeof(int));
    }
}

void init_history(TraversalHistory *history)
{
    history->count = 0;
    for (int i = 0; i < MAX_HISTORY; i++)
    {
        history->snapshots[i].visited = NULL;
        history->snapshots[i].queue_stack = NULL;
        history->snapshots[i].tree_edges.N = 0;
        history->snapshots[i].tree_edges.data = NULL;
    }
}

void save_state(TraversalHistory *history, const TraversalState *current_state)
{
    if (history->count < MAX_HISTORY)
    {
        copy_traversal_state(&history->snapshots[history->count], current_state);
        history->count++;
    }
}

bool undo_state(TraversalHistory *history, TraversalState *current_state)
{
    if (history->count > 0)
    {
        history->count--;
        copy_traversal_state(current_state, &history->snapshots[history->count]);
        return true;
    }
    return false;
}

void free_history(TraversalHistory *history)
{
    for (int i = 0; i < history->count; i++)
    {
        free_traversal_state(&history->snapshots[i]);
    }
    history->count = 0;
}