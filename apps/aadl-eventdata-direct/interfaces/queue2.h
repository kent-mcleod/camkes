#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#define BUF_SIZE 100
#define QUEUE_LEN 5  // should be based on max_queue variable

#define QINDEX(i) (i % QUEUE_LEN)
#define QLEN(fq) (fq->q->head - fq->tail)

struct q_e {
    size_t len;
    char data[BUF_SIZE];
};

struct queue {
    uint64_t head;
    struct q_e elt[QUEUE_LEN];
};

struct full_queue {
    struct queue *q;
    uint64_t tail;
};    

typedef struct queue queue_t;
typedef struct full_queue full_queue_t;

void queue_init(queue_t *q); 
void queue_enqueue(queue_t *q, void *data, size_t len); 
void queue_dequeue(full_queue_t *fq, void *data, size_t *len); 
bool queue_is_empty(full_queue_t *q); 

