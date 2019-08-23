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
#include <sel4/sel4.h>

int aadl_raise_event(void) {
    (*counter)++;
    e_emit();
    return 0;
}


int run(void) {

    int i = 0;
    int err = 0;

    while (1) {

        err = aadl_raise_event();
	if (!err) {
	    i++;
	    if (i % 7 == 0) {
	        //seL4_Yield();
                printf("%s: rasied events %d\n", get_instance_name(), i);
	    }
	} else {
            ZF_LOGE("Sender: failed to raise event %d", i);
        }
    }
}

