#pragma once
#include "vectors.h"

struct mat3
{
	float elements[3 * 3];
	mat3();
	mat3(const float k);
	mat3(const float m0, const float m1, const float m2,
		const float m3, const float m4, const float m5, 
		const float m6, const float m7, const float m8 );
	mat3(const mat3& m);
	friend mat3 transpose(const mat3& m);
	mat3& operator = (const mat3& other);
	friend const float determinant(const mat3& m);
	friend mat3 inverse(const mat3& m);
	friend const mat3 operator + (const mat3& m0, const mat3& m1);
	friend const mat3 operator - (const mat3& m0, const mat3& m1);
	friend const mat3 operator * (const float k, const mat3& m);
	friend const mat3 operator * (const mat3& m, const float k);
	friend const vec3 operator * (const mat3& m, const vec3& v);
	friend const mat3 operator * (const mat3& m0, const mat3& m1);
	friend const bool operator == (const mat3& m0, const mat3& m1);
	friend const bool operator != (const mat3& m0, const mat3& m1);
	friend std::ostream& operator << (std::ostream& os, const mat3& m);
};

