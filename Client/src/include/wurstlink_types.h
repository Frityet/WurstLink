//
// Created by Frityet on 2022-03-17.
//

#pragma once

#ifndef __cplusplus
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef bool
#undef bool
#undef true
#undef false
#endif

typedef enum Bool: _Bool { true = 1, false = 0, TRUE = 1, FALSE = 0 } bool;

//typedef void *nullptr_t;
//const nullptr_t nullptr = NULL;

#else
#include <cstdint>
#endif

typedef uint8_t byte_t;

struct EmbeddedInfo {
    const uint64_t  length;
    const byte_t    source[];
};

//typedef const struct EmbeddedInfo embedded_t;
#define EMBEDDED extern const struct EmbeddedInfo
