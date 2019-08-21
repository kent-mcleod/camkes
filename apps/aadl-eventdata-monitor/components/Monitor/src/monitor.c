/*
 * Copyright 2017, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#include <camkes.h>
#include <stdio.h>
#include <string.h>

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
} q = {.head=0, .tail=0, .len=0};
    

void sevent_raise(dataport_ptr_t ptr, size_t len) {
    int do_emit = 0;
    size_t e_len;
    char *s = (char *)dataport_unwrap_ptr(ptr);
    m_lock();
    if (q.len < QUEUE_LEN) {
	e_len = MIN(len, BUF_SIZE);
        memcpy(q.elt[q.head].data, dataport_unwrap_ptr(ptr), e_len);
	q.elt[q.head].len = e_len;
	q.head = (q.head + 1) % QUEUE_LEN;
	q.len++;
	//printf("Monitor: event_raise; added event head %d, tail %d, len %d\n", q.head, q.tail, q.len);
	do_emit = 1;
    } else {
	// drop silently
        printf("Monitor: event_raise - *dropped* event q.len: %d, data: \"%s\"\n", q.len, s);
    }
    m_unlock();

    if (do_emit) {
	e_emit();
    }
}

int revent_get_eventdata(dataport_ptr_t *ptr, size_t *len) {
    int new_tail = 0;
    int res = 0;

    m_lock();
    if (q.len > 0) {
        memcpy(dr, q.elt[q.tail].data, q.elt[q.tail].len);
	*len = q.elt[q.tail].len;
	*ptr = dataport_wrap_ptr(dr);
	new_tail = (q.tail +1) % QUEUE_LEN;
	q.tail = new_tail;
	q.len--;
	// printf("Monitor: get_eventdata; new head %d, tail %d, len %d\n", q.head, q.tail, q.len);
	res = 1;
    } else {
	// printf("Monitor: get_eventdata: no eventdata available q.len: %d\n", q.len);
	res = 0;
    }
    m_unlock();

    return res;
}

