#ifndef QUATERNIONS_H
#define QUATERNIONS_H

#include <Matrices.h>
#include <Vectors.h>
#include <iostream>
#include <string>


#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

	using namespace std;

	struct qtrn
	{
		float t, x, y, z;

		const float threshold = (float)1.0e-5;

		qtrn();
		qtrn(float tI, float xI, float yI, float zI);

		qtrn& operator=(const qtrn& q);

		static const qtrn qFromAngleAxis(float theta, vec4 axis);
		friend const void qToAngleAxis(const qtrn& q, float& theta, vec4& axis);
		friend const void clean(qtrn& q);
		friend const float quadrance(const qtrn& q);
		friend const float norm(const qtrn& q);
		friend const qtrn normalize(const qtrn& q);
		friend const qtrn conjugate(const qtrn& q);
		friend const qtrn inverse(const qtrn& q);

		friend const qtrn operator*(const qtrn& q0, const qtrn& q1);
		friend const qtrn operator*(const qtrn& q, const float s);
		friend const qtrn operator*(const float s, const qtrn& q);

		friend const qtrn multiply(const qtrn& q, const float s);
		friend const qtrn multiply(const qtrn& q0, const qtrn& q1);
		friend const void qGLMatrix(const qtrn& q, mat4& matrix);
		friend const mat4 matrixFromQtrn(const qtrn& q);
		friend const qtrn qLerp(const qtrn& q0, const qtrn& q1, float k);
		friend const qtrn qSlerp(const qtrn& q0, const qtrn& q1, float k);
		friend const bool qEqual(const qtrn& q0, const qtrn& q1);
		friend const void print(const std::string& s, const qtrn& q);
		friend const void printAngleAxis(const std::string& s, const qtrn& q);

	};



#endif