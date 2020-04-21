/*
 * Copyright 2019, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#include <autoconf.h>
#include <camkes.h>
#include <vmlinux.h>
#include <sel4vm/guest_vm.h>

#include <sel4vmmplatsupport/drivers/cross_vm_connection.h>
#include <sel4vmmplatsupport/drivers/pci_helper.h>
#include <pci/helper.h>

#ifdef CONFIG_PLAT_QEMU_ARM_VIRT
#define CONNECTION_BASE_ADDRESS 0xDF000000
#else
#define CONNECTION_BASE_ADDRESS 0x3F000000
#endif

// The following device definitions correspond to the following camkes interfaces.

// dataport queue_t sender_dataport;
// consumes SendEvent sender_event_recv;

// dataport queue_t vmSender_dataport;
// consumes SendEvent vmSender_event_recv;


// dataport queue_t receiver_dataport;
// emits SendEvent receiver_event_send;

// dataport queue_t vmReceiver_dataport;
// emits SendEvent vmReceiver_event_send;

#define NUM_CONNECTIONS 4
static struct camkes_crossvm_connection connections[NUM_CONNECTIONS];

// these are defined in the dataport's glue code
extern dataport_caps_handle_t sender_dataport_handle;
seL4_Word sender_event_recv_notification_badge(void);

extern dataport_caps_handle_t vmSender_dataport_handle;
seL4_Word vmSender_event_recv_notification_badge(void);

extern dataport_caps_handle_t receiver_dataport_handle;
void receiver_event_send_emit_underlying(void); 

extern dataport_caps_handle_t vmReceiver_dataport_handle;
void done_emit_underlying(void);


static int consume_callback(vm_t *vm, void *cookie)
{
    consume_connection_event(vm, (seL4_Word) cookie, true);
    return 0;
}


void init_cross_vm_connections(vm_t *vm, void *cookie)
{
    connections[0] = (struct camkes_crossvm_connection) {
        .handle = &sender_dataport_handle,
        .emit_fn = NULL,
        .consume_badge = sender_event_recv_notification_badge()
    };


    connections[1] = (struct camkes_crossvm_connection) {
        .handle = &vmSender_dataport_handle,
        .emit_fn = NULL,
        .consume_badge = vmSender_event_recv_notification_badge()
    };


    connections[2] = (struct camkes_crossvm_connection) {
        .handle = &receiver_dataport_handle,
        .emit_fn = receiver_event_send_emit_underlying,
        .consume_badge = -1
    };

    connections[3] = (struct camkes_crossvm_connection) {
        .handle = &vmReceiver_dataport_handle,
        .emit_fn = done_emit_underlying,
        .consume_badge = -1
    };

    for (int i = 0; i < NUM_CONNECTIONS; i++) {
        if (connections[i].consume_badge != -1) {
            int err = register_async_event_handler(connections[i].consume_badge, consume_callback, (void *)connections[i].consume_badge);
            ZF_LOGF_IF(err, "Failed to register_async_event_handler for init_cross_vm_connections.");

        }   
    }


    cross_vm_connections_init(vm, CONNECTION_BASE_ADDRESS, connections, ARRAY_SIZE(connections));
}

DEFINE_MODULE(cross_vm_connections, NULL, init_cross_vm_connections)
