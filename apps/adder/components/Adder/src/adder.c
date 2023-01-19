/*
 * Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <camkes.h>
#include <stdio.h>
#include <camkes/dataport.h>
#include "payload.h"

void a_calculate(struct payload *p1)
{
    struct payload *p2;
    // p1 = (struct payload *)dataport_unwrap_ptr(ptr);
    p2 = (struct payload *)((void *)d + 2048);
    const char *name = get_instance_name();
    p2->result = 0;
    for (int i = 0; i < p1->sz; i++) {
        printf("%s: Adding %d\n", name, p1->operands[i]);
        p2->result += p1->operands[i];
    }

    // return dataport_wrap_ptr((void *)p2);
}

void irq_handle(void)
{
    /* Clear status bit. */
    // irq_acknowledge();
    printf("got irq...\n");
    struct payload *p = (void *)d + 1024;
    a_calculate(p);
    irq2_emit_underlying();

    // seL4_DebugSendIPI(1, 4);

}

int run(void)
{

    printf("hello world1!\n");



    while (1) {
        irq_wait();
        irq_handle();
    }

}