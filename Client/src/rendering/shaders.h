#pragma once

#include "wurstlink_types.h"

#define SHADER(_name) $shaders_##_name

EMBEDDED_t SHADER(test), SHADER(fragment), SHADER(vertex);
