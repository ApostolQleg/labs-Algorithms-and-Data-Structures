#include "lab5_state.h"

void init_traversal_state(TraversalState *state, int n) {
    state->n = n;
    
    state->visited = (int *)calloc(n, sizeof(int));
    state->queue_stack = (int *)calloc(n, sizeof(int));
    
    state->tree_edges = create_matrix(n); 
    
    state->head = 0;
    state->tail = 0;
    state->current_node = -1;
    state->is_finished = false;
}

void free_traversal_state(TraversalState *state) {
    if (state->visited != NULL) {
        free(state->visited);
    }
    if (state->queue_stack != NULL) {
        free(state->queue_stack);
    }
    if (state->tree_edges != NULL) {
        destroy_matrix(state->tree_edges, state->n); 
    }
}