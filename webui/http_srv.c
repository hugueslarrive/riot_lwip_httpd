/*
 * Copyright (c) 2021 Hugues Larrive <hlarrive@pm.me>
 *
 * SPDX-License-Identifier: CECILL-2.1
 */

#include "http_srv.h"

static const char* cgi_handler(
    int idx, int num_params, char *params[], char *values[]);
static uint16_t ssi_handler(
    int idx, char *insert, int insert_len, void *connection_state);

static const tCGI cgi_handlers[] = {
    {"/", cgi_handler},
};

static const char* cgi_handler(
    int idx, int num_params, char *params[], char *values[])
{
    LWIP_ASSERT("check index", (unsigned)idx < LWIP_ARRAYSIZE(cgi_handlers));

    if (num_params >= 1) {
        if (!strcmp(params[0], "json")) {
            return "/handler.ssi";
        }
        else return "/404.html";
    }
    return "/404.html";
}

const char* ssi_tags[] = {
    "json",
};

static uint16_t ssi_handler(
    int idx, char *insert, int insert_len, void *connection_state)
{
    size_t printed;
    am2301_json_t json;

    LWIP_UNUSED_ARG(connection_state);

    switch (idx) {
        case 0: /* "json" */
            am2301_json(json);
            printed =
                snprintf(insert, insert_len, "%s", json);
            break;
        default: /* unknown tag */
            printed = 0;
            break;
    }
    LWIP_ASSERT("sane length", printed <= 0xFFFF);
    return (uint16_t)printed;
}

void http_srv_init(void)
{
    http_set_cgi_handlers(cgi_handlers, ARRAY_SIZE(cgi_handlers));

    for (unsigned i = 0; i < LWIP_ARRAYSIZE(ssi_tags); i++) {
        LWIP_ASSERT("tag too long for LWIP_HTTPD_MAX_TAG_NAME_LEN",
            strlen(ssi_tags[i]) <= LWIP_HTTPD_MAX_TAG_NAME_LEN);
    }

    http_set_ssi_handler(ssi_handler,
        ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
}
