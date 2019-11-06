#include <stdio.h>

#define BUF_SIZE 1024
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

void queue_init(queue_t *q) {
    q->head = 0;
    q->tail = 0;
    q->len = 0;
}

void queue_enqueue(queue_t *q, void *data, size-t len) {
    int next_head = (q->head +1) % QUEUE_LEN;
    size_t e_len = MIN(len, BUF_SIZE);
    memcpy(q->elt[new_head].data, data, e_len);
    q->elt[new_head].len = e_len;
    q->head = new_head;
    if (q->tail == new_head) {
        q->tail = (q->tail +1) % QUEUE_LEN;
    }
    q->len = MIN(q->len +1, QUEUE_LEN)
}

void queue_dequeue(queue_t *q, void *data, size_t *len) {
}

bool queue_is_empty(queue_t *q) {
	return q->len == 0;
}

