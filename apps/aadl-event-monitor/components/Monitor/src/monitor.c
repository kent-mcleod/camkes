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

int num_events = 0;

void sevent_raise(void) {
    int do_emit = 0;
    (void)m_lock();
    if (num_events < max_queue) {
        num_events++;
	do_emit = 1;
    }
    (void)m_unlock();
    if (do_emit) {
        e_emit();
    }
}

int revent_get_events(void) {
    (void)m_lock();
    int ne = num_events;
    num_events = 0;
    (void)m_unlock();
    return ne;
}

