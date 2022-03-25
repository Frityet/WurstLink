//
// Created by Frityet on 2022-03-01.
//

#pragma once

#define Vector2_t(_type) struct { _type x, y; }
#define VECTOR2(_x, _y) (Vector2_t) { .x = (_x), .y = (_y) }
typedef struct Vector2 {
    double x, y;
} Vector2_t;

#define Vector3_t(_type) struct { _type x, y, z; }
#define VECTOR3(_x, _y, _z) (Vector3_t) { .x = (_x), .y = (_y), .z = (_z) }
typedef struct Vector3 {
    double x, y, z;
} Vector3_t;

#define Quaternion_t(_type) struct { _type x, y, z, w; }
#define QUATERNION(_x, _y, _z, _w) (Quaternion_t) { .x = (_x), .y = (_y), .z = (_z), .w = (_w) }
typedef struct Quaternion {
    double x, y, z, w;
} Quaternion_t;


