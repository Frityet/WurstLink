//
// Created by Frityet on 2022-03-01.
//

#pragma once

#include <stdio.h>
#include <stdlib.h>

#define Vector2_ut(_type) union { struct { _type x, y; }; _type coordinates[2]; }
#define VECTOR2(_x, _y) (Vector2_ut) { .x = (_x), .y = (_y) }
typedef union Vector2 {
    struct {
        double x, y;
    };
    double coordinates[2];
} Vector2_ut;

#define Vector3_ut(_type) union { struct { _type x, y, z; }; _type coordinates[3]; }
#define VECTOR3(_x, _y, _z) (Vector3_t) { .x = (_x), .y = (_y), .z = (_z) }
#define VECTOR3_ut(_type, _x, _y, _z) (_type) { .x = (_x), .y = (_y), .z = (_z) }
typedef union Vector3 {
    struct {
        double x, y, z;
    };
    double coordinates[3];
} Vector3_ut;

#define Quaternion_ut(_type) union { struct { _type x, y, z, w; }; _type coordinates[4]; }
#define QUATERNION(_x, _y, _z, _w) (Vector3_t) { .x = (_x), .y = (_y), .z = (_z), .w = (_w) }
typedef struct Quaternion {
    struct {
        double x, y, z, w;
    };
    double coordinates[4];
} Quaternion_ut;

static inline Vector2_ut vector2_add(Vector2_ut vec1, Vector2_ut vec2)
{
    return (Vector2_ut) {
        .x = vec1.x + vec2.x,
        .y = vec2.y + vec2.y
    };
}

static inline Vector2_ut vector2_sub(Vector2_ut vec1, Vector2_ut vec2)
{
    return (Vector2_ut) {
        .x = vec1.x - vec2.x,
        .y = vec2.y - vec2.y
    };
}

static inline Vector2_ut vector2_mul(Vector2_ut vec1, Vector2_ut vec2)
{
    return (Vector2_ut) {
        .x = vec1.x * vec2.x,
        .y = vec2.y * vec2.y
    };
}

static inline Vector2_ut vector2_div(Vector2_ut vec1, Vector2_ut vec2)
{
    return (Vector2_ut) {
        .x = vec1.x / vec2.x,
        .y = vec2.y / vec2.y
    };
}


static inline char *vector2_str(Vector2_ut vec)
{
    char *ret = malloc(sizeof(char) * 1024);
    snprintf(ret, 1024, "(x: %lf, y: %lf)", vec.x, vec.y);
    return ret;
}

static inline Vector3_ut vector3_add(Vector3_ut vec1, Vector3_ut vec2)
{
    return (Vector3_ut) {
        .x = vec1.x + vec2.x,
        .y = vec1.y + vec2.y,
        .z = vec1.z + vec2.z,
    };
}

static inline Vector3_ut vector3_mul(Vector3_ut vec1, Vector3_ut vec2)
{
    return (Vector3_ut) {
        .x = vec1.x * vec2.x,
        .y = vec1.y * vec2.y,
        .z = vec1.z * vec2.z,
    };
}

static inline Vector3_ut vector3_div(Vector3_ut vec1, Vector3_ut vec2)
{
    return (Vector3_ut) {
        .x = vec1.x / vec2.x,
        .y = vec1.y / vec2.y,
        .z = vec1.z / vec2.z,
    };
}

static inline char *vector3_str(Vector3_ut vec)
{
    char *ret = malloc(sizeof(char) * 1024);
    snprintf(ret, 1024, "(x: %lf, y: %lf, z: %lf)", vec.x, vec.y, vec.z);
    return ret;
}

