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

int next_buf(void) {
    return (curr_buf + 1) % 3;
}

void *acq_write(void) {
    curr_buf = next_buf();
    if (curr_buf == *curr_reading) {
        curr_buf = next_buf();
    }
    curr_p = bufs[curr_buf];
}

void rel_write(void) {
    *last_written = curr_buf;
}


int run(void) {
    const char *name = get_instance_name();

    init_bufs();

    while (1) {
        acq_write();

        sprintf(curr_p, "%s: this is my messsage", name);

        printf("%s: wrote message \"%s\" to buf[%d] (%p)\n", name, curr_p, curr_buf, curr_p);

        rel_write();
    }

    return 0;
}
