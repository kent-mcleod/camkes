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

// TODO: this should periodic or sporadic
// periodic: react to timer event
// sporadic: react to arbitrary event
// sender must be higher prio than receiver?

#define MAX_BUF 2048
char buf[MAX_BUF];

void do_run(void);

void copy_buf_out(void) {
    memcpy(d, buf, MAX_BUF);
}

int run(void) {
    while (1) {
        do_run();
	copy_buf_out();
	//?? yield();
    }

    return 0;
}

void do_run(void) {
    sprintf(buf, "%s: this is my messsage", get_instance_name());
    printf("%s: wrote message \"%s\" to %p\n", get_instance_name(), buf, buf);
}

