/*
 * Copyright 2020, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */
/*- do configuration[me.instance.name].update({"%s_access" % me.interface.name: "R"}) -*/

/*- include 'seL4SharedDataWithCaps.template.c' -*/

/*- set connection_type = configuration[me.instance.name].get('%s_type' % me.interface.name, 'native') -*/
/*- set generate_notifications = configuration[me.parent.name].get("with_signalling", false) -*/
/*- if generate_notifications -*/

	/*- if connection_type == 'native' -*/
	    /*- include 'seL4Notification-to.template.c' -*/
	/*- elif connection_type == 'vm' -*/
		/*- from 'global-endpoint.template.c' import allocate_cap with context -*/

		/*- do allocate_cap(me, is_reader=True) -*/
		/*- set badge = pop('badge') -*/


	/*- else -*/
	     /*? raise(TemplateError('Setting %s.%s_type is not valid. Must be either "native" or "vm"' % (me.instance.name, me.interface.name))) ?*/
	/*- endif -*/
/*- endif -*/
/*- if connection_type == 'vm' -*/
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

static struct camkes_crossvm_connection connection = {
    &/*? me.interface.name ?*/_handle, NULL,
/*- if generate_notifications -*/
    /*? badge ?*/,
/*- else -*/
	-1,
/*- endif -*/


    "/*? me.interface.name ?*/",
};
CROSS_VM_CONNECTION(/*? me.interface.name ?*/, connection);

/*- if generate_notifications -*/

static int consume_callback(vm_t *vm, void *cookie)
{
    consume_connection_event(vm, connection.consume_badge, true);
    return 0;
}
/*- endif -*/

WEAK void init_cross_vm_connections(vm_t *vm, void *cookie);

static void init_connections(vm_t *vm, void *cookie)
{
/*- if generate_notifications -*/

	int err = register_async_event_handler(connection.consume_badge, consume_callback, NULL);
    ZF_LOGF_IF(err, "Failed to register_async_event_handler for init_cross_vm_connections.");
/*- endif -*/

    if (!init_cross_vm_connections) {
    	cross_vm_connections_init(vm, CONNECTION_BASE_ADDRESS, NULL, 0);
    }
}

DEFINE_MODULE(/*? me.interface.name ?*/_module, NULL, init_connections)

/*- endif -*/
