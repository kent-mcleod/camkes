/*
 * Copyright 2017, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * Copyright 2019 Adventium Labs
 * Modifications made to original
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_Adventium_BSD)
 */

#include <camkes.h>
#include <stdio.h>
#include <string.h>
#include <sel4/sel4.h>
#include <stdlib.h>
#include <counter.h>

//------------------------------------------------------------------------------
// Implemention of AADL Output Event Port named "ep1_out"

int ep1_out_aadl_send_event(void) {
    // ep1_out_counter is a dataport (shared memory) that is written by the sender and read by the reciever(s).
    // This counter is monotonicly increasing, but can wrap.
    ++(*ep1_out_counter);
    // Release memory fence - ensure subsequent write occurs after any preceeding read or write
    ep1_out_counter_release();
    // NOTE: If we only need polling style recivers, we can get rid of the SendEvent
    ep1_out_SendEvent_emit();
    return 0;
}

//------------------------------------------------------------------------------
// Testing

int run(void) {

    counter_t i = 0;
    int err = 0;

    while (1) {

        // Busy loop to slow things down
        for(unsigned int j = 0; j < 100000; ++j){
            seL4_Yield();
        }

        // Send a random number of events
        int n = (random() % 10);
        for(unsigned int j = 0; j < n; ++j){
            ++i;
            printf("%s: sending event %" PRIuMAX "\n", get_instance_name(), i);
            err = ep1_out_aadl_send_event();
	    if (err) {
                ZF_LOGE("%s: failed to raise event %" PRIuMAX ", exiting", get_instance_name(), i);
                return err;
            }
        }
    }
}

// Emacs Declarations
// Local Variables:
// mode: c
// c-basic-offset: 4
// indent-tabs-mode: nil
// End:              


