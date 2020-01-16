#include "Vectors.h"
#include "Matrices.h"

#pragma once
class Camera
{
public:

	vec3 Eye;
	vec3 Center;
	vec3 Up;
	vec3 View;
	vec3 Right;

	mat4 ViewMatrix;
	mat4 ProjectionMatrix;

	Camera();
	Camera(vec3 eye, vec3 center, vec3 up);

	void update();
	void update(vec3 eye, vec3 center, vec3 up);

};

