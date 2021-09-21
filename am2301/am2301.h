/*
 * Copyright (c) 2021 Hugues Larrive <hlarrive@pm.me>
 *
 * SPDX-License-Identifier: CECILL-2.1
 */

#ifndef AM2301_H
#define AM2301_H

#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "dht.h"
#include "dht_params.h"
#include "periph/cpuid.h"

typedef struct {
    int temp;
    int rh;
} am2301_data_t;

typedef char am2301_json_t[43];

void *am2301_thread(void *arg);

int am2301_init(void);

void am2301_json(char* json);

#endif /* AM2301_H */
