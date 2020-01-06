#include "../Include/vectors.h"


vec3 convert2DTo3D(const vec2& other) 
{
	return vec3(other.x, other.y, 0.0f);
}

vec2 convert3DTo2D(const vec3& other) 
{
	return vec2(other.x/other.z, other.y/other.z);
}

vec4 convert3DTo4D(const vec3& other)
{
	return vec4(other.x, other.y, other.z, 1.0f);
}

vec4 convert2DTo4D(const vec2& other) 
{
	return vec4(other.x, other.y, 0.0f, 1.0f);
}

vec2 convert4DTo2D(const vec4& other)
{
	return convert3DTo2D(convert4DTo3D(other));
}
vec3 convert4DTo3D(const vec4& other) 
{
	return vec3(other.x/other.w, other.y/other.w, other.z/other.w);
}