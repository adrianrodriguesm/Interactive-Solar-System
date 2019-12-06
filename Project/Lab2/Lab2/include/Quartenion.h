#pragma once
#include <string>
#include "vec4.h"
#include "mat4.h"
using namespace mathsMat4;
#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913
namespace mathsQtrn
{
	const float qThreshold = (float)1.0e-5;
	struct qtrn
	{
		
		float t, x, y, z;
		qtrn();
		qtrn(float t,float x,float y,float z);
		static const qtrn qFromAngleAxis(const float& theta, const vec4& axis);
		static const void qToAngleAxis(const qtrn& q, float& theta, vec4& axis);

		friend const void qClean(qtrn& q);
		friend const float qQuadrance(const qtrn& q);
		friend const float qNorm(const qtrn& q);
		friend const qtrn qNormalize(const qtrn& q);
		friend const qtrn qConjugate(const qtrn& q);
		friend const qtrn qInverse(const qtrn& q);
		friend const qtrn qAdd(const qtrn& q0, const qtrn& q1);
		friend const qtrn qMultiply(const qtrn& q, const float s);
		friend const qtrn qMultiply(const qtrn& q0, const qtrn& q1);
		friend const void qGLMatrix(const qtrn& q,mat4& matrix);
		friend const qtrn qLerp(const qtrn& q0, const qtrn& q1, float k);
		friend const qtrn qSlerp(const qtrn& q0, const qtrn& q1, float k);
		friend const bool qEqual(const qtrn& q0, const qtrn& q1);
		friend const void qPrint(const std::string& s, const qtrn& q);
		friend const void qPrintAngleAxis(const std::string& s, const qtrn& q);

		qtrn& operator = (const qtrn& other);
		friend const qtrn operator + (const qtrn& left, const qtrn& right);
		friend const qtrn operator * (const qtrn& q, const float s);
		friend const qtrn operator * (const qtrn& q0, const qtrn& q1);
		friend const bool operator == (const qtrn& left, const qtrn& right);
		friend const bool operator != (const qtrn& left, const qtrn& right);
		friend std::ostream& operator<<(std::ostream& stream, const qtrn& q0);
	};
	
}