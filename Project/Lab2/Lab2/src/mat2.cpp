#include "../include/mat2.h"
#include <math.h>

namespace mathsMat2
{
	float DELTA = 0.001f;
	mat2::mat2() {
		for (int i = 0; i < 4; i++) {
			this->elements[i] = 0.0f;
		}

		this->elements[0] = this->elements[3] = 1.0f;
	}

	mat2::mat2(float k) {
		for (int i = 0; i < 4; i++) {
			this->elements[i] = 0.0f;
		}
		this->elements[0] = this->elements[3] = k;
	}

	mat2::mat2(const float m0, const float m1, const float m2, const float m3) {

		// 2x2 matrix - column major
		//    0    2
		//    1    3
		this->elements[0] = m0;
		this->elements[2] = m2;
		this->elements[1] = m1;
		this->elements[3] = m3;
	}

	mat2::mat2(const mat2& m) {
		for (int i = 0; i < 4; i++) {
			this->elements[i] = m.elements[i];
		}
	}

	mat2 transpose(const mat2& m) {
		//    0    2   
		//    1    3    
		mat2 transpose;
		transpose.elements[0] = m.elements[0];
		transpose.elements[1] = m.elements[2];
		transpose.elements[2] = m.elements[1];
		transpose.elements[3] = m.elements[3];
		return transpose;
	}
	mat2& mat2::operator = (const mat2& other) {
		for (int i = 0; i < 4; i++)
		{
			this->elements[i] = other.elements[i];
		}
		return *this;
	}

	const float determinant(const mat2& m) {

		float det = m.elements[0] * m.elements[3] - m.elements[1] * m.elements[2];
		return det;

	}
	mat2 inverse(const mat2& m) {
		mat2 inverse;
		float inv[4], det;
		
		det = m.elements[0] * m.elements[3] - m.elements[1] * m.elements[2];

		if (det == 0.0f) {
			return false;
		}

		det = 1.0f / det;
		inv[0] = m.elements[3];
		inv[1] = -(m.elements[1]);
		inv[2] = -(m.elements[2]);
		inv[3] = m.elements[0];
		for (int i = 0; i < 4; i++) {
			inverse.elements[i] = inv[i] * det;
		}
		return inverse;
	}

	const mat2 operator + (const mat2& m0, const mat2& m1) {
		mat2 result;
		for (int i = 0; i < 4; i++) {
			result.elements[i] = m0.elements[i] + m1.elements[i];
		}
		return result;
	}

	const mat2 operator - (const mat2& m0, const mat2& m1) {
		mat2 result;
		for (int i = 0; i < 4; i++) {
			result.elements[i] = m0.elements[i] - m1.elements[i];
		}
		return result;
	}

	const mat2 operator * (const float k, const mat2& m) {
		mat2 result;
		for (int i = 0; i < 4; i++) {
			result.elements[i] = m.elements[i] * k;
		}
		return result;
	}

	const mat2 operator * (const mat2& m, const float k) {
		mat2 result;
		for (int i = 0; i < 4; i++) {
			result.elements[i] = m.elements[i] * k;
		}
		return result;
	}
	const vec2 operator * (const mat2& m, const vec2& v) {
		vec2 result;
		result.x = v.x * m.elements[0] + v.y * m.elements[0 + 1 * 2];
		result.y = v.x * m.elements[1 + 0 * 2] + v.y * m.elements[1 + 1 * 2];	
		return result;
	}

	const mat2 operator * (const mat2& m0, const mat2& m1) {
		mat2 result;
		for (int y = 0; y < 2; y++) {
			for (int x = 0; x < 2; x++) {
				float sum = 0.0f;
				for (int e = 0; e < 2; e++) {
					sum += m0.elements[x + e * 2] * m1.elements[e + y * 2];
				}
				result.elements[x + y * 2] = sum;
			}
		}
		return result;
	}
	const bool operator == (const mat2& m0, const mat2& m1) {
		for (int i = 0; i < 4; i++) {
			float comp = m0.elements[i] - m1.elements[i];
			if (comp > DELTA) {
				return false;
			}
		}
		return true;
	}

	const bool operator != (const mat2& m0, const mat2& m1) {
		return !(m0 == m1);
	}

	std::ostream& operator << (std::ostream& os, const mat2& m) {
		os << "[" << m.elements[0] << "," << m.elements[2] << std::endl
			<< m.elements[1] << "," << m.elements[3] << "]" << std::endl;
		return os;
	}
}