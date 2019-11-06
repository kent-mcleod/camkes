#pragma once

#include <unistd.h>
#include <stdbool.h>

#define BUF_SIZE 100
#define QUEUE_LEN 5  // should be based on max_queue variable

struct q_e {
    size_t len;
    char data[BUF_SIZE];
};

struct queue {
    int head;
    int tail;
    int len;
    struct q_e elt[QUEUE_LEN];
};

typedef struct queue queue_t;

void queue_init(queue_t *q); 
void queue_enqueue(queue_t *q, void *data, size_t len); 
void queue_dequeue(queue_t *q, void *data, size_t *len); 
bool queue_is_empty(queue_t *q); 

