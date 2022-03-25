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

typedef enum Bool: _Bool { true = 1, false = 0 } bool;
#else
#include <cstdint>
#endif

typedef uint8_t byte_t;
