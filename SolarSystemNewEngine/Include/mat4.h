#pragma once
#include "vectors.h"

const float mThreshold = (float)1.0e-5;
	
struct mat4
{
		
	float elements[4 * 4];
	mat4();
	mat4(float k);
	mat4(const float m0, const float m1, const float m2, const float m3,
		const float m4, const float m5, const float m6, const float m7,
		const float m8, const float m9, const float m10, const float m11,
		const float m12, const float m13, const float m14, const float m15);
	mat4(const mat4& m);
	friend mat4 transpose(const mat4& m);
	mat4& operator = (const mat4& other);
	friend const float determinant(const mat4& m);
	friend mat4 inverse(const mat4& m);
	friend const mat4 operator + (const mat4& m0, const mat4& m1);
	friend const mat4 operator - (const mat4& m0, const mat4& m1);
	friend const mat4 operator * (const float k, const mat4& m);
	friend const mat4 operator * (const mat4& m, const float k);
	friend const vec4 operator * (const mat4& m, const vec4& v);
	friend const mat4 operator * (const mat4& m0, const mat4& m1);
	friend const bool operator == (const mat4& m0, const mat4& m1);
	friend const bool operator != (const mat4& m0, const mat4& m1);
	friend std::ostream& operator << (std::ostream& os, const mat4& m);

	friend const void mClean(mat4& m);

		
}; 


