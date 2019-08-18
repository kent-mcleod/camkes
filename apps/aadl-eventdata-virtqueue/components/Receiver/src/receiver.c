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
//#include <camkes/dataport.h>

#include <virtqueue.h>
#include <camkes/virtqueue.h>


void aadl_event_data_port_handler(void *data, size_t len) {
    printf("Receiver received: %s, len: %d\n", (char*)data, len);
}

virtqueue_device_t * vq1;

int virtqueue_init(void) {
    static int vq1_initialised = 0;

    /* Initialise virtqueue vq1 */
    if (vq1_initialised) {
        return 0;
    }
    int err = camkes_virtqueue_device_init(&vq1, 0);
    if (err) {
        ZF_LOGE("Unable to initialise Receiver vq1");
        return 1;
    } else {
	vq1_initialised = 1;
	return 0;
    }
}

static void virtqueue_send(virtqueue_device_t *vq, volatile void * buf, size_t buf_size)
{
    int enqueue_res = virtqueue_device_enqueue(vq, buf, buf_size);
    if (enqueue_res) {
        ZF_LOGE("Receiver send: virtqueue enqueue failed");
        return;
    }

    int err = virtqueue_device_signal(vq);
    if (err != 0) {
        ZF_LOGE("Receiver send: virtqueue signal failed");
        return;
    }
}


void handle_vq_callback(virtqueue_device_t *vq)
{
    volatile void* buf = NULL;
    size_t buf_size = 0;
    int err = virtqueue_device_dequeue(vq, &buf, &buf_size);
    if (err) {
        ZF_LOGE("Receiver callback: virtqueue dequeue failed");
        return;
    }

    aadl_event_data_port_handler((void*)buf, buf_size);

    /* Send the buffer back */
    virtqueue_send(vq, buf, buf_size);
}


void vq_wait_callback(void) {
//    printf("Receiver: vq_wait_callback\n");

    virtqueue_init();

    int vq1_poll_res = virtqueue_device_poll(vq1);
    if (vq1_poll_res) {
        handle_vq_callback(vq1);
    }
    if (vq1_poll_res == -1) {
        ZF_LOGF("Receiver vq1 poll failed");
    }
}


