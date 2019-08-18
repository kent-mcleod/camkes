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

#include <virtqueue.h>
#include <camkes/virtqueue.h>


uint64_t hash_copy;

void post_init(void) {
    *hashd = 0;
}

void acq_write(void) {
    hash_copy = *hashd;
    *hashd = 0;
}

void rel_write(void) {
    *hashd = hash_copy + 1;
}

int virtqueue_init(void); 
void vq_send(char *s, int len); 

char p[1024];

int run(void) {

    virtqueue_init();

    int i = 0;

    while (1) {

        sprintf(p, "%s: this is my messsage %d", get_instance_name(), i++);
	printf("%s: writing %s\n", get_instance_name(), p);

	vq_send(p, strlen(p));

        printf("%s: wrote message \"%s\"\n", get_instance_name(), p);
    }
}

virtqueue_driver_t * vq1;

void handle_vq_callback(virtqueue_driver_t *vq);

void vq_send(char *s, int len) {
    volatile void *alloc_buffer = NULL;

    /* Check if there is data still waiting in the virtqueue */
    int poll_res = virtqueue_driver_poll(vq1);
    if (poll_res) {
        handle_vq_callback(vq1);
    }

    int err = camkes_virtqueue_buffer_alloc(vq1, &alloc_buffer, len);
    if (err) {
        return;
    }

    char *buffer_data = (char *)alloc_buffer;
    memcpy(buffer_data, s, len);

    err = virtqueue_driver_enqueue(vq1, alloc_buffer, len);
    if (err != 0) {
        ZF_LOGE("Sender vq1 enqueue failed");
        camkes_virtqueue_buffer_free(vq1, alloc_buffer);
        return;
    }

    err = virtqueue_driver_signal(vq1);
    if (err != 0) {
        ZF_LOGE("Sender vq1 signal failed");
        return;
    }

}


void handle_vq_callback(virtqueue_driver_t *vq) {
    volatile void* buf = NULL;
    size_t buf_size = 0;
    int err = virtqueue_driver_dequeue(vq, &buf, &buf_size);
    if (err) {
        ZF_LOGE("Sender virtqueue dequeue failed");
        return;
    }
    /* Clean up and free the buffer we allocated */
    camkes_virtqueue_buffer_free(vq, buf);
}


void vq_wait_callback(void) {
    printf("vq_wait_callback\n");

    int err;

    int vq1_poll_res = virtqueue_driver_poll(vq1);
    if (vq1_poll_res) {
        handle_vq_callback(vq1);
    }
    if (vq1_poll_res == -1) {
        ZF_LOGF("Sender vq1 poll failed");
    }

}

int virtqueue_init(void) {
    /* Initialise virtqueue vq1 */
    int err = camkes_virtqueue_driver_init(&vq1, 0);
    if (err) {
        ZF_LOGE("Unable to initialise vq1");
        return 1;
    }

}
