//Header File

#ifndef VECTORS_H
#define VECTORS_H

#include <iostream>
#include <vector>
#include <math.h>
#include <string>


using namespace std;
class vec4;

class vec2 {

public:
	float x, y;

	//Default Constructor:
	vec2();

	//Overloaded Constructors:
	vec2(const float x);
	vec2(const float x, const float y);
	vec2(const vec2& v);

	void clean();

	//Functions:
	float ValByIndex(int i) const;
	float ValByAxis(char c) const;
	float Magnitude() const;
	float MagnitudeSqrd() const;

	friend const vec2 normalize(const vec2& v);
	friend const vec2 operator+(const vec2 v1, const vec2 v2);
	friend const vec2 operator-(const vec2 v1, const vec2 v2);
	friend const vec2 operator*(const vec2 v1, const float a);
	friend const vec2 operator*(const float a, const vec2 v1);
	friend const bool operator==(const vec2 v1, const vec2 v2);
	friend const bool operator!=(const vec2 v1, const vec2 v2);

	vec2& operator+=(const vec2& v2);
	vec2& operator-=(const vec2& v2);
	vec2& operator*=(const float a);

	friend const float Dot(const vec2 v1, const vec2 v2);
	friend const float CosAngle(const vec2 v1, const vec2 v2);
	friend const double Angle(const vec2 v1, const vec2 v2);

	friend std::ostream& operator<<(std::ostream& os, const vec2 v);
	friend std::istream& operator>>(std::istream& is, vec2& v);

	vec2 ProjectOn(vec2 v2);

};

class vec3 {

public:

	float x, y, z;

	//Default Constructor:
	vec3();

	//Overloaded Constructors:
	vec3(const float x);
	vec3(const float x, const float y);
	vec3(const float x, const float y, const float z);
	vec3(const vec2 v);
	vec3(const vec4 v);
	vec3(const vec3& v);

	//Functions:
	float Magnitude() const;
	float MagnitudeSqrd() const;

	friend const vec3 normalize(const vec3& v);
	friend const vec3 operator+(const vec3 v1, const vec3 v2);
	friend const vec3 operator-(const vec3 v1, const vec3 v2);
	friend const vec3 operator*(const vec3 v1, const float a);
	friend const vec3 operator*(const float a, const vec3 v1);
	friend const bool operator==(const vec3 v1, const vec3 v2);
	friend const bool operator!=(const vec3 v1, const vec3 v2);

	vec3& operator+=(const vec3& v2);
	vec3& operator-=(const vec3& v2);
	vec3& operator*=(const float a);

	friend const float Dot(const vec3 v1, const vec3 v2);
	friend const vec3 Cross(const vec3 v1, const vec3 v2);
	friend const float CosAngle(const vec4 v1, const vec4 v2);
	friend const double Angle(const vec4 v1, const vec4 v2);
	friend const vec3 vecLerp(const vec3 vStart, const vec3 vEnd, const float amount);

	friend std::ostream& operator<<(std::ostream& os, const vec3 v);
	friend std::istream& operator>>(std::istream& is, vec3& v);

	vec2 Tovec2();
	vec4 Tovec4();
};

class vec4 {

public:

	float x, y, z, w;

	//Default Constructor:
	vec4();

	//Overloaded Constructors:
	vec4(const float x);
	vec4(const float x, const float y);
	vec4(const float x, const float y, const float z);
	vec4(const float x, const float y, const float z, const float w);
	vec4(const vec4& vec4);
	vec4(const vec3 vec3);
	vec4(const vec3 v, const float inw);
	vec4(const vec2 v);

	//Functions:
	float Magnitude() const;
	float MagnitudeSqrd() const;

	friend const vec4 normalize(const vec4& v);
	friend const vec4 operator+(const vec4 v1, const vec4 v2);
	friend const vec4 operator-(const vec4 v1, const vec4 v2);
	friend const vec4 operator*(const vec4 v1, const float a);
	friend const vec4 operator*(const float a, const vec4 v1);
	friend const bool operator==(const vec4 v1, const vec4 v2);
	friend const bool operator!=(const vec4 v1, const vec4 v2);

	vec4& operator+=(const vec4& v2);
	vec4& operator-=(const vec4& v2);
	vec4& operator*=(const float a);

	friend const float Dot(const vec4 v1, const vec4 v2);
	friend const float Dot3D(const vec4 v1, const vec4 v2);
	friend const vec4 Cross(const vec4 v1, const vec4 v2);
	friend const float CosAngle(const vec4 v1, const vec4 v2);
	friend const double Angle(const vec4 v1, const vec4 v2);

	friend std::ostream& operator<<(std::ostream& os, const vec4 v);
	friend std::istream& operator>>(std::istream& is, vec4& v);

	vec3 Tovec3();

};

#endif // !VECTOROPERATIONS_H

