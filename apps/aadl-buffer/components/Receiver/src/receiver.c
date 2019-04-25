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
//#include "payload.h"
#include <camkes/dataport.h>

void *bufs[3];
int curr_buf = 0;
void *curr_p;
int *last_written;
int *curr_reading;

void init_bufs(void) {
    bufs[0] = d1;
    bufs[1] = d2;
    bufs[2] = d3;
    curr_buf = 0;
    curr_p = bufs[curr_buf];
    last_written = (int *)dw;
    curr_reading = (int *)dr;
}

void *acq_read(void) {
    curr_buf = *last_written;
    curr_p = bufs[curr_buf];
    *curr_reading = curr_buf; 
}

void rel_read(void) {
    *curr_reading = -1;
}

int run(void) {
    const char *name = get_instance_name();

    init_bufs();

    int len = 0;

    //while (len == 0) {
    while (1) {
	acq_read();
	len = strlen(curr_p);
    	printf("%s: read message (len: %d) \"%s\" from buf[%d] (%p)\n", name, len, curr_p, curr_buf, curr_p);
	rel_read();
    }

    return 0;
}
