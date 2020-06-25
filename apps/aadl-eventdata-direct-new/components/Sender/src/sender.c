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
#include <data.h>
#include <queue.h>


//------------------------------------------------------------------------------
// Implementation of AADL Input Event Data Port (out) named "p1_out"
//
// NOTE: If we only need polling style receivers, we can get rid of the queue

// Assumption: only one thread is calling this and/or reading p1_in_recv_counter.
void p1_out_aadl_event_data_send(data_t *data) {
    queue_enqueue(p1_out_queue, data);
    p1_out_queue_emit_underlying();
}

//------------------------------------------------------------------------------
// Testing

void post_init(void) {
    queue_init(p1_out_queue);
}

int run(void) {

    int i = 0;
    int err = 0;
    data_t data;

    while (1) {

        // Busy loop to slow things down
        for(unsigned int j = 0; j < 100000; ++j){
            seL4_Yield();
        }

        // Send a random number of data elements
        int n = (random() % 10);
        for(unsigned int j = 0; j < n; ++j){
            ++i;
            // Stage data
            data.x = i;
            printf("%s: sending: %d\n", get_instance_name(), data.x);
            // Send the data
            p1_out_aadl_event_data_send(&data);          
        }
    }
}

// Emacs Declarations
// Local Variables:
// mode: c
// c-basic-offset: 4
// indent-tabs-mode: nil
// End:              


