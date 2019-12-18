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

/*
 * Copyright 2019 Adventium Labs.
 * Modifications made to original.
 */

#include <camkes.h>
#include <stdio.h>
#include <stdlib.h>
#include <counter.h>

//------------------------------------------------------------------------------
// Implemention of AADL Input Event Port named "ep1_in"
// Three styles: polling, waiting and callback.
//
// Callback would typicaly be avoid for safety critical systems. It is harder
// to analyze since the callback handler is run on some arbitrary thread.

counter_t ep1_in_recv_counter = 0;

// Assumption: only one thread is calling this and/or reading ep1_in_recv_counter.
bool ep1_in_aadl_event_poll(void) {
    counter_t the_counter = *ep1_in_counter;
    // Acquire memory fence - ensure preceding read occurs before any subsequent read or write
    ep1_in_counter_acquire();
    if ( the_counter != ep1_in_recv_counter ) {
        // NOTE: Counters can wrap, so we must use != above instead of >.  If
        // it ever happens that reciever falls so far behind that the send
        // counter wraps all the way around, the reciever will loose all those
        // events without knowing it. Since the counter is large this would be
        // a very exceptional situation where something has gone very wrong
        // with the receiver.
        ++ep1_in_recv_counter;
        return true;
    }
    return false;
}

void ep1_in_aadl_event_wait(void) {
    while (!ep1_in_aadl_event_poll()) {
        ep1_in_SendEvent_wait();
    }
}

void ep1_in_aadl_event_port_handler(void) {
    printf("%s: received event %" PRIuMAX "\n", get_instance_name(), ep1_in_recv_counter);
}

static void ep1_in_handler(void *v) {
    // Handle ALL events that have been queued up
    while (ep1_in_aadl_event_poll()) {
    	ep1_in_aadl_event_port_handler();
    }
    while (! ep1_in_SendEvent_reg_callback(&ep1_in_handler, NULL));
}

//------------------------------------------------------------------------------
// Testing

int run_callback(void) {
    return ep1_in_SendEvent_reg_callback(&ep1_in_handler, NULL);
}

void run_wait(void) {
    while (true) {
        ep1_in_aadl_event_wait();
        printf("%s: received event %" PRIuMAX "\n", get_instance_name(), ep1_in_recv_counter);
    }
}

void run_poll(void) {
    while (1) {

        // Busy loop slow things down
        for(unsigned int j = 0; j < 80000; ++j){
            seL4_Yield();
        }

        // Random number of polls for testing
        int n = (random() % 10);
        for(unsigned int j = 0; j < n; ++j){
            bool eventReceived = ep1_in_aadl_event_poll();
            if (eventReceived) {
                printf("%s: received event %" PRIuMAX "\n", get_instance_name(), ep1_in_recv_counter);
            } else {
                printf("%s: received no events\n", get_instance_name());
            }
        }

    }

}

int run(void) {
    // Pick one receive style to test.
    //return run_callback();
    //run_wait();
    run_poll();
}

// Emacs Declarations
// Local Variables:
// mode: c
// c-basic-offset: 4
// indent-tabs-mode: nil
// End:              
