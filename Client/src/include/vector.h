//
// Created by Frityet on 2022-03-01.
//

#pragma once

#include "managed.h"

#define Vector2_t(_type) struct { _type x, y; }
#define VECTOR2(...) (Vector2_t) { __VA_ARGS__ }
typedef struct Vector2 {
    double x, y;
} Vector2_t;

#define Vector3_t(_type) struct { _type x, y, z; }
typedef struct Vector3 {
    double x, y, z;
} Vector3_t;

#define Quaternion_t(_type) struct { _type x, y, z, w; }
typedef struct Quaternion {
    double x, y, z, w;
} Quaternion_t;

static inline Vector2_t vector2_add(Vector2_t vec1, Vector2_t vec2)
{
    return (Vector2_t) {
        .x = vec1.x + vec2.x,
        .y = vec2.y + vec2.y
    };
}

static inline Vector2_t vector2_sub(Vector2_t vec1, Vector2_t vec2)
{
    return (Vector2_t) {
        .x = vec1.x - vec2.x,
        .y = vec2.y - vec2.y
    };
}

static inline Vector2_t vector2_mul(Vector2_t vec1, Vector2_t vec2)
{
    return (Vector2_t) {
        .x = vec1.x * vec2.x,
        .y = vec2.y * vec2.y
    };
}

static inline Vector2_t vector2_div(Vector2_t vec1, Vector2_t vec2)
{
    return (Vector2_t) {
        .x = vec1.x / vec2.x,
        .y = vec2.y / vec2.y
    };
}


static inline char *vector2_str(Vector2_t vec)
{
    char *ret = managed_alloc(sizeof(char) * 1024, NULL);
    snprintf(ret, 1024, "(x: %lf, y: %lf)", vec.x, vec.y);
    return ret;
}

static inline Vector3_t vector3_add(Vector3_t vec1, Vector3_t vec2)
{
    return (Vector3_t) {
        .x = vec1.x + vec2.x,
        .y = vec1.y + vec2.y,
        .z = vec1.z + vec2.z,
    };
}

static inline Vector3_t vector3_mul(Vector3_t vec1, Vector3_t vec2)
{
    return (Vector3_t) {
        .x = vec1.x * vec2.x,
        .y = vec1.y * vec2.y,
        .z = vec1.z * vec2.z,
    };
}

static inline Vector3_t vector3_div(Vector3_t vec1, Vector3_t vec2)
{
    return (Vector3_t) {
        .x = vec1.x / vec2.x,
        .y = vec1.y / vec2.y,
        .z = vec1.z / vec2.z,
    };
}

static inline char *vector3_str(Vector3_t vec)
{
    char *ret = managed_alloc(sizeof(char) * 1024, NULL);
    snprintf(ret, 1024, "(x: %lf, y: %lf, z: %lf)", vec.x, vec.y, vec.z);
    return ret;
}

