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

#include <camkes.h>
#include <stdio.h>
#include <string.h>

#include <virtqueue.h>
#include <camkes/virtqueue.h>



virtqueue_driver_t * vq1;

void handle_vq_callback(virtqueue_driver_t *vq);

int vq_send(char *s, int len) {
    volatile void *alloc_buffer = NULL;

    /* Check if there is data still waiting in the virtqueue */
    int poll_res = virtqueue_driver_poll(vq1);
    if (poll_res) {
        handle_vq_callback(vq1);
    }

    int err = camkes_virtqueue_buffer_alloc(vq1, &alloc_buffer, len);
    if (err) {
        ZF_LOGE("Sender: vq1 buffer alloc failed");
        return 1;
    }

    char *buffer_data = (char *)alloc_buffer;
    memcpy(buffer_data, s, len);

    err = virtqueue_driver_enqueue(vq1, alloc_buffer, len);
    if (err != 0) {
        ZF_LOGE("Sender: vq1 enqueue failed");
        camkes_virtqueue_buffer_free(vq1, alloc_buffer);
        return 1;
    }

    err = virtqueue_driver_signal(vq1);
    if (err != 0) {
        ZF_LOGE("Sender: vq1 signal failed");
        return 1;
    }

    return 0;

}


void handle_vq_callback(virtqueue_driver_t *vq) {
    volatile void* buf = NULL;
    size_t buf_size = 0;
    int err = virtqueue_driver_dequeue(vq, &buf, &buf_size);
    if (err) {
        ZF_LOGE("Sender: virtqueue dequeue failed");
        return;
    }
    /* Clean up and free the buffer we allocated */
    camkes_virtqueue_buffer_free(vq, buf);
}


void vq_wait_callback(void) {
    // printf("Sender: vq_wait_callback\n");

    int err;

    int vq1_poll_res = virtqueue_driver_poll(vq1);
    if (vq1_poll_res) {
        handle_vq_callback(vq1);
    }
    if (vq1_poll_res == -1) {
        ZF_LOGF("Sender: vq1 poll failed");
    }

}

int virtqueue_init(void) {
    /* Initialise virtqueue vq1 */
    int err = camkes_virtqueue_driver_init(&vq1, 0);
    if (err) {
        ZF_LOGE("Sender: Unable to initialise vq1");
        return 1;
    } else {
	return 0;
    }

}

int aadl_raise_event_data(void *data, size_t len) {
    return vq_send(data, len);
}

#define BUF_SIZE 1024

char buf[BUF_SIZE];

int run(void) {

    virtqueue_init();

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
    }
}
