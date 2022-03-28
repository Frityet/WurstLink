//
// Created by Frityet on 2022-03-25.
//

#pragma once

#include <stdio.h>

#include "logger.h"
#include "list.h"

static char *file_readlines_n(file, buf, len)
            FILE *file;
            size_t len;
            char buf[static len];
{
    rewind(file);

    for (size_t i = 0; i < len
                       //&& buf[i] != '\0' --if it ain't null terminated, that ur problem lol.
    ; i++) {
        buf[i] = getc(file);

        if (feof(file))
            break;

        if (ferror(file)) {
            LOG_ERROR("Could not read from file!");
            rewind(file);
            return NULL;
        }
    }

    rewind(file);
    return buf;
}

static inline char *file_readlines(FILE *file, char *buf)
{
    return file_readlines_n(file, buf, strlen(buf));
}

static inline char *file_readlines_a(FILE *file)
{
    char *buf = LIST(char);

    rewind(file);
    while (true) {
        char c = getc(file);
        PUSH_LIST(buf, c);

        if (feof(file))
            break;

        if (ferror(file)) {
            LOG_ERROR("Could not read from file!");
            rewind(file);
            return NULL;
        }
    }
    rewind(file);

    char *ret = list_to_array(buf);
    free_list(buf);
    return ret;
}
