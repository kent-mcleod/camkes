#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#include <queue2.h>

#ifndef MIN
#define MIN(a,b) ((a < b) ? a: b)
#endif

#ifndef MAX
#define MAX(a,b) ((a > b) ? a: b)
#endif

void queue_init(queue_t *q) {
    q->head = 0;
}

void queue_enqueue(queue_t *q, void *data, size_t len) {
    uint64_t next_head = (q->head +1);
    int nhi = QINDEX(next_head);
    size_t e_len = MIN(len, BUF_SIZE);
    memcpy(q->elt[nhi].data, data, e_len);
    q->elt[nhi].len = e_len;
    q->head = next_head;
}

void queue_dequeue(full_queue_t *fq, void *data, size_t *len) {
    fq->tail = MAX(fq->tail, fq->q->head - QUEUE_LEN);
    int ti = QINDEX(fq->tail);
    memcpy(data, fq->q->elt[ti].data, fq->q->elt[ti].len);
    *len = fq->q->elt[ti].len;
    fq->tail++;
}

bool queue_is_empty(full_queue_t *q) {
    return QLEN(q) == 0;
}

