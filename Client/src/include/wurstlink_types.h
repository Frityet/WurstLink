//
// Created by Frityet on 2022-03-17.
//

#pragma once


#define nullable    _Nullable
#define nonnull     _Nonnull
#define nullable_ptr *nullable
#define nonnull_ptr *nonnull

#ifndef __cplusplus
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef bool
#undef bool
#undef true
#undef false
#endif

#include "common.h"

typedef enum Bool: _Bool { true = 1, false = 0, TRUE = 1, FALSE = 0 } bool;


#else
#include <cstdint>
#endif

#define SHARED_STRUCT(_name, _type, _sname, _arrname, ...) union _name { struct { _type __VA_ARGS__; } _sname; _type _arrname[VA_ARGS_COUNT(__VA_ARGS__)]; }
SHARED_STRUCT(Test, int, values, members, a, b, c);

typedef uint8_t byte_t;

struct EmbeddedInfo {
    const uint64_t  length;
    const byte_t    source[];
};

#define EMBEDDED_t extern const struct EmbeddedInfo

