/*
 * Copyright (c) 2021 Hugues Larrive <hlarrive@pm.me>
 *
 * SPDX-License-Identifier: CECILL-2.1
 */

#include "am2301.h"

static dht_t dev;
static am2301_data_t data;
static int cpuid;

static char am2301_stack[THREAD_STACKSIZE_SMALL];

void *am2301_thread(void *arg)
{
    (void) arg;

    xtimer_ticks32_t last_wakeup = xtimer_now();

    int16_t temp, rh;

    while(1) {
        if (dht_read(&dev, &temp, &rh) != DHT_OK) {
            puts("Error reading AM2301 values");
            continue;
        }
        //~ printf("\t%0.1f°C\t%0.1f%%\n", (double)temp / 10, (double)rh / 10);
        data.temp = (int)temp;
        data.rh = (int)rh;

        xtimer_periodic_wakeup(&last_wakeup, 2 * US_PER_SEC);
    }

    return NULL;
}

int am2301_init(void)
{
    if (dht_init(&dev, &dht_params[0]) != DHT_OK)
        return 1;
    (void) thread_create(
            am2301_stack, sizeof(am2301_stack),
            THREAD_PRIORITY_MAIN - 1,
            THREAD_CREATE_WOUT_YIELD | THREAD_CREATE_STACKTEST,
            am2301_thread, NULL, "am2301");
    cpuid_get(&cpuid);
    return 0;
}

void am2301_json(am2301_json_t json)
{
    sprintf(json,
        "{\"cpuid\":\"%x\",\"temp\":%d,\"rh\":%d}",
        cpuid, data.temp, data.rh);  
}
