#pragma once
#include "Quartenion.h"
#include "matFactory.h"
using namespace mathsQtrn;

namespace EngineCamera {
	struct Camera
	{
	private:
		qtrn qRot;
		float d;	
		const vec4 axis_x = { 1.0f, 0.0f, 0.0f, 1.0f };
		const vec4 axis_y = { 0.0f, 1.0f, 0.0f, 1.0f };
		const vec4 axis_z = { 0.0f, 0.0f, 1.0f, 1.0f };
	public:
		///Matrices
		mat4 viewMatrix;
		mat4 projMatrix;

		Camera();
		Camera(const vec3& eye, const vec3& direction, const vec3& up);//LookAt
		
		///Update de Matrices
		void setPersProjMatrix(const float fov, const float aspectRatio, const float near, const float far);
		void setOrthoProjMatrix(const float left, const float right, 
								const float bottom, const float top, 
								const float near,  const float far);
		void setRotMatrix(float xOffset, float yOffset);
		void setTranslationMatrix(const float d);
			
	};
}