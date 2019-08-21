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
#include <camkes/dataport.h>

int num_events = 0;

void aadl_event_data_port_handler(void *data, size_t len) {
    printf("Receiver received event data %d: len %d, string \"%s\"\n", num_events, len, data);
    num_events++;
}

static void handler(void *v) {
    dataport_ptr_t ptr;
    size_t len;
    int has_data;
    // printf("Receiver: callback\n");
    do {
        has_data = event_get_eventdata(&ptr, &len);
	if (has_data) {
	   aadl_event_data_port_handler(dataport_unwrap_ptr(ptr), len);
	}
    } while (has_data);
    e_reg_callback(&handler, NULL);
}

int run(void) {
    e_reg_callback(&handler, NULL);
    return 0;
}

