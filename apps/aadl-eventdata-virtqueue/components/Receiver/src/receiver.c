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


#define BUF_SIZE 2048
char buf[BUF_SIZE];
uint64_t prev_hash = 0;

// This still has a problem in that if the read fails for whatever
// reason (i.e. the read started during a write, or the sender made a 
// write during the read) then the receiver will use the most
// recently *read* buffer.  Ideally it should read the most recently
// *written* buffer.
// 
// Here's an example of the undesireable behaviour:
//
// sender: wrote message "sender: this is my messsage 89" to 0x145000 (hashd: 180, 0x146000)
// sender: writing sender: this is my messsage 90 (hashd: 181)
// sender: wrote message "sender: this is my messsage 90" to 0x145000 (hashd: 182, 0x146000)
// receiver: read message (len: 30) "sender: this is my messsage 90" from 0x133570 (hashd: 182 0x147000)
// ...
// sender: writing sender: this is my messsage 91 (hashd: 183)
// sender: wrote message "sender: this is my messsage 91" to 0x145000 (hashd: 184, 0x146000)
// ...
// sender: wrote message "sender: this is my messsage 99" to 0x145000 (hashd: 200, 0x146000)
// sender: writing sender: this is m
// receiver: read message (len: 30) "sender: this is my messsage 90" from 0x133570 (hashd: 201 0x147000)
// ...
//
// receiver reads message "90", while really, it should be reading message "99"
//

void do_read(void *p, int size) {
    char b[size];
    uint64_t h;

    h = *hashd;
    if (h == prev_hash) return; // nothing has changed
    if (h == 0) return; // the sender is writing

    memcpy(b, d, size);

    if (h == *hashd) {
	// nothing changed during reading
        memcpy(p, b, size);
	prev_hash = h;
    }
}

int virtqueue_init(void);

/*
int run(void) {

    int len = 0;

    while (1) {

        do_read(buf, BUF_SIZE);

        len = strlen(buf);
        printf("%s: read message (len: %d) \"%s\" from %p (hashd: %llu %p)\n", get_instance_name(), len, buf, buf, *hashd, hashd);
    }

    return 0;
}
*/

virtqueue_device_t * vq1;

static void virtqueue_send(virtqueue_device_t *vq, volatile void * buf, size_t buf_size)
{
    int enqueue_res = virtqueue_device_enqueue(vq, buf, buf_size);
    if (enqueue_res) {
        ZF_LOGE("Receiver virtqueue enqueue failed");
        return;
    }

    int err = virtqueue_device_signal(vq);
    if (err != 0) {
        ZF_LOGE("Receiver signal failed");
        return;
    }
}


void handle_vq_callback(virtqueue_device_t *vq)
{

    volatile void* buf = NULL;
    size_t buf_size = 0;
    int err = virtqueue_device_dequeue(vq, &buf, &buf_size);
    if (err) {
        ZF_LOGE("Receiver virtqueue dequeue failed");
        return;
    }

    char *read_buffer = (char *)buf;
    printf("Receiver received: %s, len: %d\n", read_buffer, buf_size);

    /* Send the buffer back */
    virtqueue_send(vq, buf, buf_size);
}

int inited = 0;

void vq_wait_callback(void) {
    printf("vq_wait_callback\n");

    if (!inited) {
    	virtqueue_init();
	inited = 1;
    }

    int err;

    int vq1_poll_res = virtqueue_device_poll(vq1);
    if (vq1_poll_res) {
        handle_vq_callback(vq1);
    }
    if (vq1_poll_res == -1) {
        ZF_LOGF("Receiver vq1 poll failed");
    }

}

int virtqueue_init(void) {
    /* Initialise virtqueue vq1 */
    int err = camkes_virtqueue_device_init(&vq1, 0);
    if (err) {
        ZF_LOGE("Unable to initialise Receiver vq1");
        return 1;
    }
}

