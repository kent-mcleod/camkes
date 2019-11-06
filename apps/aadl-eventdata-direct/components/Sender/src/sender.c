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

int aadl_send_data(void *data, size_t len) {
    queue_enqueue(queue, data, len);
    e_emit();
    return 0;
}


int run(void) {

    int i = 0;
    int err = 0;

    queue_init(queue);

    while (1) {

        err = aadl_send_data(&i, sizeof(i));
	if (!err) {
	    i++;
	    if (i % 7 == 0) {
	        //seL4_Yield();
                printf("%s: sent data %d\n", get_instance_name(), i);
	    }
	} else {
            ZF_LOGE("Sender: failed to send data %d", i);
        }
    }
}

