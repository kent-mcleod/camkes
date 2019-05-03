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

void post_init(void) {
    *hashd = 0;
}

void acq_write(void) {
    (*hashd)++;
}

void rel_write(void) {
    (*hashd)++;
}

int run(void) {
    int i = 0;
    char *p = (char *)d;

    while (1) {
	acq_write();

        sprintf(p, "%s: this is my messsage %d", get_instance_name(), i++);
	printf("%s: writing %s (hashd: %llu)\n", get_instance_name(), p, *hashd);

        rel_write();

        printf("%s: wrote message \"%s\" to %p (hashd: %llu, %p)\n", get_instance_name(), p, p, *hashd, hashd);
    }
}

