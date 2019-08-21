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

int num_events = 0;

void aadl_event_port_handler(void) {
    printf("Receiver received event %d\n", num_events);
    num_events++;
}

static void handler(void *v) {
    int ne = event_get_events();
    for (int i = 0; i < ne; i++) {
    	aadl_event_port_handler();
    }
    while (! e_reg_callback(&handler, NULL));
}

int run(void) {
    return e_reg_callback(&handler, NULL);
}

