#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <queue.h>

#ifndef MIN
#define MIN(a,b) ((a < b) ? a: b)
#endif
/*
#define MAX(a,b) ((a > b) ? a: b)
*/

void queue_init(queue_t *q) {
    q->head = 0;
    q->tail = 0;
    q->len = 0;
}

void queue_enqueue(queue_t *q, void *data, size_t len) {
    //printf("enqueue (before): data: %p, len: %d, head: %d, tail: %d, elt: %p, len: %d\n", data, q->len, q->head, q->tail, q->elt[q->tail].data, q->elt[q->tail].len); 
    int new_head = (q->head +1) % QUEUE_LEN;
    size_t e_len = MIN(len, BUF_SIZE);
    memcpy(q->elt[new_head].data, data, e_len);
    q->elt[new_head].len = e_len;
    q->head = new_head;
    if ((q->tail == new_head) && (q->len != 0)) {
        q->tail = (q->tail +1) % QUEUE_LEN;
    }
    q->len = MIN(q->len +1, QUEUE_LEN);
    //printf("enqueue (after): data: %p, len: %d, head: %d, tail: %d, elt: %p, len: %d\n", data, q->len, q->head, q->tail, q->elt[q->tail].data, q->elt[q->tail].len); 
}

void queue_dequeue(queue_t *q, void *data, size_t *len) {
    if (q->len <= 0) {
        return;
    }
    //printf("dequeue (before): data: %p, len: %d, head: %d, tail: %d, elt: %p, len: %d\n", data, q->len, q->head, q->tail, q->elt[q->tail].data, q->elt[q->tail].len); 
    memcpy(data, q->elt[q->tail].data, q->elt[q->tail].len);
    *len = q->elt[q->tail].len;
    q->tail = (q->tail +1) % QUEUE_LEN;
    q->len--;
    printf("dequeue (after): data: %p, len: %d, head: %d, tail: %d, elt: %p, len: %d\n", data, q->len, q->head, q->tail, q->elt[q->tail].data, q->elt[q->tail].len); 
}

bool queue_is_empty(queue_t *q) {
	return q->len <= 0;
}

