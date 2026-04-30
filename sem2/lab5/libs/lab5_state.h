#ifndef LAB5_STATE_H
#define LAB5_STATE_H

#include <stdbool.h>
#include <stdlib.h>
#include "graph_lib.h"

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

void init_traversal_state(TraversalState *state, int n);
void free_traversal_state(TraversalState *state);

#endif 