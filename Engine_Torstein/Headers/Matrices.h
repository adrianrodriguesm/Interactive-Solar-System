#ifndef MATRICES_H
#define MATRICES_H

#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <vectors.h>

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

using namespace std;

const float EPSILON = 0.00001;

typedef float GLMatrix[16];
struct mat2;
struct mat3;
struct mat4;
struct MatrixFactory;

struct mat2 {
	float data[4];

	//Constructors:
	mat2();
	mat2(const float a);
	mat2(const float m0, const float m1,
		const float m2, const float m3);
	mat2(const mat2& m);
		
	//Functions:
	void clean();
	float determinant() const;
	friend mat2 transpose(const mat2& m);
	friend mat2 inverse(const mat2& m);
	mat2& operator=(const mat2& m);
	mat2& operator+=(const mat2& m);
	mat2& operator-=(const mat2& m);
	friend const bool operator==(const mat2& m0, const mat2& m1);
	friend const bool operator!=(const mat2& m0, const mat2& m1);
	friend const mat2 operator+(const mat2& m0, const mat2& m1);
	friend const mat2 operator-(const mat2& m0, const mat2& m1);

	friend const mat2 operator*(const mat2& m0, const mat2& m1);
	friend const mat2 operator*(const mat2& m0, const float a);
	friend const mat2 operator*(const float a, const mat2& m0);
	friend const vec2 operator*(const mat2& m, const vec2& v);

	friend std::ostream& operator<<(std::ostream& os, const mat2 m);
	friend std::istream& operator>>(std::istream& is, mat2& m);

};

struct mat3 {
	float data[9];

	//Constructors:
	mat3();
	mat3(const float a);
	mat3(const float m0, const float m1, const float m2,
		const float m3, const float m4, const float m5, 
		const float m6, const float m7, const float m8);
	mat3(const vec3& v1, const vec3& v2, const vec3& v3);
	mat3(const mat3& m);

	//Functions:
	void clean();
	float determinant() const;
	friend mat3 transpose(const mat3& m);
	friend mat3 inverse(const mat3& m);
	mat3& operator=(const mat3& m);
	mat3& operator+=(const mat3& m);
	mat3& operator-=(const mat3& m);
	friend const bool operator==(const mat3& m0, const mat3& m1);
	friend const bool operator!=(const mat3& m0, const mat3& m1);
	friend const mat3 operator+(const mat3& m0, const mat3& m1);
	friend const mat3 operator-(const mat3& m0, const mat3& m1);

	friend const mat3 operator*(const mat3& m0, const mat3& m1);
	friend const mat3 operator*(const mat3& m0, const float a);
	friend const mat3 operator*(const float a, const mat3& m0);
	friend const vec3 operator*(const mat3& m, const vec3& v);

	friend std::ostream& operator<<(std::ostream& os, const mat3 m);
	friend std::istream& operator>>(std::istream& is, mat3& m);

};

struct mat4 {
	float data[16];
	typedef float GLMatrix[16];

	//Constructors:
	mat4();
	mat4(const float a);
	mat4(const float m0, const float m1, const float m2, const float m3,
		const float m4, const float m5, const float m6, const float m7, 
		const float m8, const float m9, const float m10, const float m11, 
		const float m12, const float m13, const float m14, const float m15);
	mat4(const mat4& m);

	//Functions:
	void clean();
	float determinant() const;
	friend mat4 transpose(const mat4& m);
	mat4& operator=(const mat4& m);
	mat4& operator+=(const mat4& m);
	mat4& operator-=(const mat4& m);
	friend const bool operator==(const mat4& m0, const mat4& m1);
	friend const bool operator!=(const mat4& m0, const mat4& m1);
	friend const mat4 operator+(const mat4& m0, const mat4& m1);
	friend const mat4 operator-(const mat4& m0, const mat4& m1);

	friend const mat4 operator*(const mat4& m0, const mat4& m1);
	friend const mat4 operator*(const mat4& m0, const float a);
	friend const mat4 operator*(const float a, const mat4& m0);
	friend const vec4 operator*(const mat4& m, const vec4& v);

	friend std::ostream& operator<<(std::ostream& os, const mat4 m);
	friend std::istream& operator>>(std::istream& is, mat4& m);

};

struct MatrixFactory {

	static const mat2 createZeroMat2();
	static const mat2 createIdentityMat2();
	static const mat2 createScaleMat2(const vec2& v);
	static const mat2 createRoationMat2(const float angle);

	static const mat3 createZeroMat3();
	static const mat3 createIdentityMat3();
	static const mat3 createDualMat3(const vec3& v);
	static const mat3 createScaleMat3(const vec3& v);
	static const mat3 createRoationMat3(const float angle, const vec3& axis);
	static const mat3 createMat3FromMat4(const mat4& m);
	static const mat3 createMat3FromMat2(const mat2& m);

	static const mat4 createZeroMat4();
	static const mat4 createIdentityMat4();
	static const mat4 createDualMat4(const vec3& v);
	static const mat4 createScaleMat4(const vec3& v);
	static const mat4 createRoationMat4(const float angle, const vec3& axis);
	static const mat4 createTranslationMat4(const vec3& v);
	static const mat4 createMat4FromMat3(const mat3& m);
	
	//Camera / World View:
	static const mat4 createViewMatrix(const vec3 eye, const vec3 center, const vec3 up);
	static const mat4 createOrthoProjectionMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far);
	static const mat4 createPerspectiveProjectionMatrix(const float fovY, const float aspect, const float near, const float far);



};



#endif