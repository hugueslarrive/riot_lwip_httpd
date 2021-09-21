/*
 * Copyright (c) 2021 Hugues Larrive <hlarrive@pm.me>
 *
 * SPDX-License-Identifier: CECILL-2.1
 */

#include "shell.h"

#include "am2301/am2301.h"
#include "webui/http_srv.h"

static const shell_command_t shell_commands[] = {
    { NULL, NULL, NULL }
};

int main(void)
{
    http_srv_init();
    if (am2301_init()) puts("Error initializing AM2301 sensor");
    
    /* run the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
