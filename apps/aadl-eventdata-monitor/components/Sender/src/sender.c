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
#include <sel4/sel4.h>


#define D_SIZE 4096

int aadl_raise_event_data(void *data, size_t len) {
    if (len > D_SIZE) {
        ZF_LOGE("Sender: data too large");
	return -1;
    }
    memcpy(d, data, len);
    event_raise(dataport_wrap_ptr(d), len);
    return 0;
}

#define BUF_SIZE 1024
char buf[BUF_SIZE];

int run(void) {

    int i = 0;
    int err = 0;

    while (1) {

        sprintf(buf, "%s: this is my messsage %d", get_instance_name(), i++);
        // printf("%s: writing \"%s\"\n", get_instance_name(), buf);

        err = aadl_raise_event_data(buf, strlen(buf));

        /*
        if (!err) {
            printf("%s: wrote message \"%s\"\n", get_instance_name(), buf);
        }
        */
        if (err) {
            ZF_LOGE("Sender: failed to raise event");
        }
	if (i % 7 == 0) {
	    seL4_Yield();
	}
    }
}


