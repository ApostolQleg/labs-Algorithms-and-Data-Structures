#ifndef LAB5_STATE_H
#define LAB5_STATE_H

#include <stdbool.h>
#include <stdlib.h>
#include "graph_lib.h"

#define MAX_HISTORY 1000

typedef struct
{
    int n;
    int *visited;
    int **tree_edges;
    int *queue_stack;
    int head;
    int tail;
    int current_node;
    bool is_finished;
} TraversalState;

typedef struct
{
    TraversalState snapshots[MAX_HISTORY];
    int count;
} TraversalHistory;

void init_traversal_state(TraversalState *state, int n);
void free_traversal_state(TraversalState *state);

void copy_traversal_state(TraversalState *dest, const TraversalState *src);

void init_history(TraversalHistory *history);
void save_state(TraversalHistory *history, const TraversalState *current_state);
bool undo_state(TraversalHistory *history, TraversalState *current_state);
void free_history(TraversalHistory *history);

#endif