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


int aadl_raise_event(void) {
    event_raise();
    return 0;
}


int run(void) {

    int i = 0;
    int err = 0;

    while (1) {

        err = aadl_raise_event();
	if (!err) {
            printf("%s: rasied event %d\n", get_instance_name(), i);
	    i++;
	} else {
            ZF_LOGE("Sender: failed to raise event %d", i);
        }
    }
}

