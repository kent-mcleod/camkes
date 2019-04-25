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


int run(void) {
    const char *name = get_instance_name();

    char *p = (char *)d;
    sprintf(p, "%s: this is my messsage", name);

    a_write(dataport_wrap_ptr((void*)p), strlen(p)+1);

    printf("%s: wrote message \"%s\"\n", name, p);
    return 0;
}
