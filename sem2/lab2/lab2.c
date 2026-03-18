#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Node
{
    int value;
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
    Node *tail;
} Queue;

Queue *create_queue();
void destroy_queue(Queue *queue);

bool is_empty(Queue *queue);

int peek(Queue *queue, bool *status);

void enqueue(Queue *queue, int value);
int dequeue(Queue *queue, bool *status);

void print_queue(Queue *queue);

void _reverse_recursive(Queue *original, Queue *reversed);
Queue *create_reversed_queue(Queue *original);

int main()
{
    Queue *q_original = create_queue();

    int n;
    printf("Enter the number of elements (n > 0): ");
    if (scanf("%d", &n) != 1 || n <= 0)
    {
        printf("Input error. The number of elements must be greater than 0.\n");
        return 1;
    }

    printf("Enter %d characters: ", n);
    for (int i = 0; i < n; i++)
    {
        char ch;
        scanf(" %c", &ch);
        enqueue(q_original, ch);
    }

    printf("\nOriginal queue: ");
    print_queue(q_original);

    Queue *q_reversed = create_reversed_queue(q_original);

    printf("Reversed queue: ");
    print_queue(q_reversed);

    destroy_queue(q_original);
    destroy_queue(q_reversed);

    return 0;
}

Queue *create_queue()
{
    Queue *queue = malloc(sizeof(Queue));

    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

bool is_empty(Queue *queue)
{
    return (queue->head == NULL);
}

int peek(Queue *queue, bool *status)
{
    if (is_empty(queue))
    {
        *status = false;
        return 0;
    }

    *status = true;
    return queue->head->value;
}

void enqueue(Queue *queue, int value)
{
    Node *newNode = malloc(sizeof(Node));

    newNode->value = value;
    newNode->next = NULL;

    if (is_empty(queue))
    {
        queue->head = newNode;
        queue->tail = newNode;
    }
    else
    {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
}

int dequeue(Queue *queue, bool *status)
{
    if (is_empty(queue))
    {
        *status = false;
        return 0;
    }

    *status = true;

    int value = queue->head->value;

    Node *oldHead = queue->head;

    if (queue->head == queue->tail)
    {
        queue->head = NULL;
        queue->tail = NULL;
    }
    else
    {
        queue->head = queue->head->next;
    }

    free(oldHead);

    return value;
}

void destroy_queue(Queue *queue)
{
    Node *currentNode = queue->head;

    while (currentNode != NULL)
    {
        Node *temp = currentNode;
        currentNode = currentNode->next;
        free(temp);
    }

    free(queue);
}

void print_queue(Queue *queue)
{
    Node *currentNode = queue->head;
    while (currentNode != NULL)
    {
        printf("%c ", currentNode->value);
        currentNode = currentNode->next;
    }
    printf("\n");
}

void _reverse_recursive(Queue *original, Queue *reversed)
{
    bool status;
    int item = dequeue(original, &status);

    if (status)
    {
        _reverse_recursive(original, reversed);
        enqueue(reversed, item);
    }
}

Queue *create_reversed_queue(Queue *original)
{
    Queue *reversed_queue = create_queue();
    _reverse_recursive(original, reversed_queue);

    return reversed_queue;
}