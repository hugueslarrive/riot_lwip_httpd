/*
 * Copyright (c) 2021 Hugues Larrive <hlarrive@pm.me>
 *
 * SPDX-License-Identifier: CECILL-2.1
 */

#ifndef HTTP_SRV_H
#define HTTP_SRV_H

#include <stdio.h>
#include <string.h>

#include "lwip/opt.h"
#include "lwip/apps/httpd.h"
#include "lwip/def.h"
#include "lwip/mem.h"

#include "am2301/am2301.h"

#define MAX_CGI_LEN   16

void http_srv_init(void);

#endif /* HTTP_SRV_H */
