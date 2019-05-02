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

#define BUF_SIZE 2048
char buf[BUF_SIZE];

bool started = false;

void do_write(void *p, int size) {
    if (!started) {
        *hashd = 1;
	started = true;
    } else {
	(*hashd)++;
    }
    memcpy(d, p, size);
    (*hashd)++;
}

int run(void) {
    int i = 0;

    while (1) {
        sprintf(buf, "%s: this is my messsage %d", get_instance_name(), i++);

        do_write(buf, strlen(buf));

        printf("%s: wrote message \"%s\" to %p (hashd: %llu, %p)\n", get_instance_name(), buf, buf, *hashd, hashd);
    }
}

