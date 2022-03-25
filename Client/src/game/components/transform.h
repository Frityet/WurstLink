//
// Created by Frityet on 2022-03-24.
//

#pragma once

#include "vector.h"

struct Transform {
    Vector3_t position, speed, velocity;
    Quaternion_t rotation;
};
