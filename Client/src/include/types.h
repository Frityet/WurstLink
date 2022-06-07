#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <mstring.h>

#undef Bool
#undef bool
#undef true
#undef false
typedef enum Bool: _Bool { true = 1, false = 0 } bool;

typedef uint8_t		uint8;
typedef uint16_t 	uint16;
typedef uint32_t 	uint32;
typedef uint64_t 	uint64;
typedef __uint128_t uint128;

typedef int8_t		int8;
typedef int16_t		int16;
typedef int32_t		int32;
typedef int64_t		int64;
typedef __int128_t 	int128;

typedef float 		float32;
typedef double 		float64;
typedef long double float128;
