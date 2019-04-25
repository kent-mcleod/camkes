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
//#include "payload.h"
#include <camkes/dataport.h>

#define BUF_SIZE 2048

int run(void) {
    const char *name = get_instance_name();

    int len = 0;
    char *p = (char *)d;
    p[0] = '\0';

    while (len == 0) {
	len = a_read(dataport_wrap_ptr((void*)p), BUF_SIZE);
    	printf("%s: read message (len: %d) \"%s\"\n", name, len, p);
    }

    return 0;
}
