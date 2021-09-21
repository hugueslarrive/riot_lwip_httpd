 /*
 * Copyright (c) 2017 Simon Goldschmidt
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Simon Goldschmidt <goldsimon@gmx.de>
 *
 */

/**
 * @file
 * Hacked file : some functions from HTTPD simple SSI example needed by
 * fs.c and httpd.c
 */
 
#include "http_srv.h"

void *fs_state_init(struct fs_file *file, const char *name)
{
    char* ret;
    LWIP_UNUSED_ARG(file);
    LWIP_UNUSED_ARG(name);
    ret = (char*)mem_malloc(MAX_CGI_LEN);
    if (ret) {
        *ret = 0;
    }
    return ret;
}

void fs_state_free(struct fs_file *file, void *state)
{
    LWIP_UNUSED_ARG(file);
    if (state != NULL) {
        mem_free(state);
    }
}

void httpd_cgi_handler( struct fs_file *file, const char* uri,
    int iNumParams, char **pcParam, char **pcValue,
    void *connection_state)
{
    LWIP_UNUSED_ARG(file);
    LWIP_UNUSED_ARG(uri);
    if (connection_state != NULL) {
        char *start = (char *)connection_state;
        char *end = start + MAX_CGI_LEN;
        int i;
        memset(start, 0, MAX_CGI_LEN);
        /* print a string of the arguments: */
        for (i = 0; i < iNumParams; i++) {
            size_t len;
            len = end - start;
            if (len) {
                size_t inlen = strlen(pcParam[i]);
                size_t copylen = LWIP_MIN(inlen, len);
                memcpy(start, pcParam[i], copylen);
                start += copylen;
                len -= copylen;
            }
            if (len) {
                *start = '=';
                start++;
                len--;
            }
            if (len) {
                size_t inlen = pcValue[i] ? strlen(pcValue[i]) : 0;
                size_t copylen = LWIP_MIN(inlen, len);
                memcpy(start, pcValue[i], copylen);
                start += copylen;
                len -= copylen;
            }
            if (len) {
                *start = ';';
                len--;
            }
            /* ensure NULL termination */
            end--;
            *end = 0;
        }
    }
}
