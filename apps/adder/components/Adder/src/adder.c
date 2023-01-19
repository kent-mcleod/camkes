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
    p2 = (struct payload *)((void *)d + 2048);
    const char *name = get_instance_name();
    p2->result = 0;
    for (int i = 0; i < p1->sz; i++) {
        printf("%s: Adding %d\n", name, p1->operands[i]);
        p2->result += p1->operands[i];
    }

}

void irq_handle(void)
{
    struct payload *p = (void *)d + 1024;
    a_calculate(p);
    irq2_emit_underlying();
}

int run(void)
{
    while (1) {
        irq_wait();
        irq_handle();
    }

}