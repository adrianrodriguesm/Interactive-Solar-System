#pragma once
#include "../include/vec2.h"
#include "../include/vec3.h"
#include "../include/vec4.h"

using  namespace mathsVec2;
using  namespace mathsVec3;
using  namespace mathsVec4;

vec3 convert2DTo3D(const vec2& other);
vec4 convert2DTo4D(const vec2& other);
vec2 convert3DTo2D(const vec3& other);
vec4 convert3DTo4D(const vec3& other);
vec2 convert4DTo2D(const vec4& other);
vec3 convert4DTo3D(const vec4& other);