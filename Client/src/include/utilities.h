//
// Created by Frityet on 2022-03-02.
//

#pragma once

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

static inline uint32_t to2ndpower(uint32_t n)
{
    return (n--, n |= n >> 1, n |= n >> 2, n |= n >> 4, n |= n >> 8, n |= n >> 16, ++n);
}
