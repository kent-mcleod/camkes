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
int my_counter = 0;

int aadl_get_events(void) {
    int dif = 0;
    int the_counter = *counter;
    if (my_counter <= the_counter) {
        dif = the_counter - my_counter;
        my_counter = the_counter;
    }
    return dif;
}
void aadl_event_port_handler(void) {
    printf("Receiver received event %d\n", num_events);
    num_events++;
}

static void handler(void *v) {
    
    int ne = aadl_get_events();
    for (int i = 0; i < ne; i++) {
    	aadl_event_port_handler();
    }
    while (! e_reg_callback(&handler, NULL));
}

int run(void) {
    return e_reg_callback(&handler, NULL);
}

