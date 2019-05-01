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

#define BUF_SIZE 2048

char buf[BUF_SIZE];
int  buf_size = 0;


void write_handler(void *_ UNUSED) {
    const char *name = get_instance_name();

    char *p = (char*)wd;

    //m_lock();
    memcpy(buf, p, BUF_SIZE);
    buf_size = BUF_SIZE;
    //m_unlock();

    printf("%s: a_write(%p, %d) invoked\n", name, p, BUF_SIZE);
    e_reg_callback(&write_handler, NULL);
}

int run(void) {
    printf("Registering callback...\n");
    e_reg_callback(&write_handler, NULL);
    return 0;
}

/*
void a_write(dataport_ptr_t ptr, int size) {
    printf("a_write unimplemented\n");
}
*/

int a_read(dataport_ptr_t ptr, int max_size) {
    assert(max_size >= buf_size);

    const char *name = get_instance_name();

    char *p;
    p = (char *)dataport_unwrap_ptr(ptr);

    //m_lock();
    memcpy(p, buf, buf_size);
    //m_unlock();

    printf("%s: a_read(%p, %d) -> %d invoked\n", name, p, max_size, buf_size);

    return buf_size;
}
