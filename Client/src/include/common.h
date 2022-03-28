//
// Created by Frityet on 2022-03-11.
//

#pragma once

#ifdef __cplusplus
#error This is a C only header!
#endif

#include <stdlib.h>

#include "wurstlink_types.h"
#include "logger.h"

#define ATTRIBUTE(...) __attribute__((__VA_ARGS__))
#define auto __auto_type
#define AUTO(_init, _free) for (_init, *_cnt_##__COUNTER__ = (void *)0; _cnt_##__COUNTER__ == (void *)0; (_free), _cnt_##__COUNTER__ = (void *)1)
//#define new(_t) (_t *)alloc_or_kablooey(sizeof(_t), __FILE__, __LINE__, __PRETTY_FUNCTION__)
//static inline void *alloc_or_kablooey(size_t typesize, const char *file, int line, const char *func)
//{
//    void *mem = calloc(1, typesize);
//    if (mem == NULL) {
//        LOG_FATAL("Could not allocate %zu bytes at [%s:%d]::%s!", typesize, file, line, func);
//        exit(EXIT_FAILURE);
//    }
//    return mem;
//}
