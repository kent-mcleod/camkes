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

// TODO: this should periodic or sporadic
// periodic: react to timer event
// sporadic: react to arbitrary event
// sender must be higher prio than receiver?

#define MAX_BUF 2048
char buf[MAX_BUF];

void do_run(void);

void copy_buf_in(void) {
    memcpy(buf, d, MAX_BUF);
}

int run(void) {
    while (1) {
        copy_buf_in();
        do_run();
        //?? yield();
    }

    return 0;
}

void do_run(void) {
    int len = strlen(buf);
    printf("%s: read message (len: %d) \"%s\" from %p\n", get_instance_name(), len, buf, buf);
}

