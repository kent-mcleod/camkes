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

char data[BUF_SIZE];
size_t len;

full_queue_t fq;

void aadl_eventdata_port_handler(void *data, size_t len) {
    printf("Receiver received data (%d) len %d\n", *(int*)data, len);
}


static void handler(void *v) {
    
    while (!queue_is_empty(&fq)) {
	   queue_dequeue(&fq, data, &len);
	   aadl_eventdata_port_handler(data, len);
    }
    printf("dequeue done\n");
    e_reg_callback(&handler, NULL);
    //while (! e_reg_callback(&handler, NULL));
    printf("handler done\n");
}

int run(void) {
    fq.q = queue;
    fq.tail = 0;
    return e_reg_callback(&handler, NULL);
}

