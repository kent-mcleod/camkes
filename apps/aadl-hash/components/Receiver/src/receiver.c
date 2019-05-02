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

void do_read(void *p, int size) {
    char b[size];
    uint64_t h;

    h = *hashd;
    memcpy(b, d, size);

    if (h == *hashd) {
        memcpy(p, b, size);
    }
}

int run(void) {
    int len = 0;

    while (1) {

        do_read(buf, BUF_SIZE);

        len = strlen(buf);
        printf("%s: read message (len: %d) \"%s\" from %p (hashd: %llu %p)\n", get_instance_name(), len, buf, buf, *hashd, hashd);
    }

    return 0;
}

