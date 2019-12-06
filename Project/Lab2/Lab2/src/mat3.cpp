#include "../include/mat3.h"
#include <math.h>

namespace mathsMat3
{
	float DELTA = 0.001f;
	mat3::mat3() {
		for (int i = 0; i < 9; i++) {
			this->elements[i] = 0.0f;
		}
		this->elements[0] = this->elements[1 + 1 * 3] = this->elements[2 + 2 * 3] = 1.0f;
	}
	mat3::mat3(const float k) {
		for (int i = 0; i < 9; i++) {
			this->elements[i] = 0.0f;
		}
		this->elements[0] = this->elements[1 + 1 * 3] = this->elements[2 + 2 * 3] = k;
	}

	mat3::mat3(const float m0, const float m1, const float m2,
		const float m3, const float m4, const float m5,
		const float m6, const float m7, const float m8) {
		// 3x3 matrix - column major
		//    0    3    6   
		//    1    4    7   
		//    2    5    8
		this->elements[0] = m0;
		this->elements[1] = m1;
		this->elements[2] = m2;

		this->elements[3] = m3;
		this->elements[4] = m4;
		this->elements[5] = m5;

		this->elements[6] = m6;
		this->elements[7] = m7;
		this->elements[8] = m8;

	}

	mat3::mat3(const mat3& m) {
		for (int i = 0; i < 9; i++) {
			this->elements[i] = m.elements[i];
		}
	}

	mat3 transpose(const mat3& m) {
		mat3 transpose;

		transpose.elements[0] = m.elements[0];
		transpose.elements[3] = m.elements[1];
		transpose.elements[6] = m.elements[2];

		transpose.elements[1] = m.elements[3];
		transpose.elements[4] = m.elements[4];
		transpose.elements[7] = m.elements[5];

		transpose.elements[2] = m.elements[6];
		transpose.elements[5] = m.elements[7];
		transpose.elements[8] = m.elements[8];

		return transpose;

	}

	mat3& mat3::operator = (const mat3& other) {
		for (int i = 0; i < 9; i++)
		{
			this->elements[i] = other.elements[i];
		}
		return *this;
	}

	const float determinant(const mat3& m) {
		float t1 = m.elements[0] * m.elements[4];
		float t2 = m.elements[0] * m.elements[7];
		float t3 = m.elements[3] * m.elements[1];
		float t4 = m.elements[6] * m.elements[1];
		float t5 = m.elements[3] * m.elements[2];
		float t6 = m.elements[6] * m.elements[2];
		float det = (t1 * m.elements[8] - t2 * m.elements[5] - t3 * m.elements[8] + t4 * m.elements[5] + t5 * m.elements[7] - t6 * m.elements[4]);
		return det;
	}

	mat3 inverse(const mat3& m) {

		mat3 inverse;
		float det = determinant(m);
		if (det == 0.0f) {
			return false;
		}

		float invd = 1.0f / det;
		
		float m0 = (m.elements[4] * m.elements[8] - m.elements[7] * m.elements[5]) * invd;
		float m3 = -(m.elements[3] * m.elements[8] - m.elements[6] * m.elements[5]) * invd;

		float m6 = (m.elements[3] * m.elements[7] - m.elements[6] * m.elements[4]) * invd;

		float m1 = -(m.elements[1] * m.elements[8] - m.elements[7] * m.elements[2]) * invd;

		float m4 = (m.elements[0] * m.elements[8] - m.elements[6] * m.elements[2]) * invd;

		float m7 = -(m.elements[0] * m.elements[7] - m.elements[6] * m.elements[1]) * invd;

		float m2 = (m.elements[1] * m.elements[5] - m.elements[4] * m.elements[2]) * invd;

		float m5 = -(m.elements[0] * m.elements[5] - m.elements[3] * m.elements[2]) * invd;

		float m8 = (m.elements[0] * m.elements[4] - m.elements[3] * m.elements[1]) * invd;

		inverse.elements[0] = m0;
		inverse.elements[3] = m3;
		inverse.elements[6] = m6;

		inverse.elements[1] = m1;
		inverse.elements[4] = m4;
		inverse.elements[7] = m7;

		inverse.elements[2] = m2;
		inverse.elements[5] = m5;
		inverse.elements[8] = m8;

		return inverse;
	}

	const mat3 operator + (const mat3& m0, const mat3& m1) {
		mat3 result;
		for (int i = 0; i < 9; i++) {
			result.elements[i] = m0.elements[i] + m1.elements[i];
		}
		return result;
	}

	const mat3 operator - (const mat3& m0, const mat3& m1) {
		mat3 result;
		for (int i = 0; i < 9; i++) {
			result.elements[i] = m0.elements[i] - m1.elements[i];
		}
		return result;
	}

	const mat3 operator * (const float k, const mat3& m) {
		mat3 result;
		for (int i = 0; i < 9; i++) {
			result.elements[i] = m.elements[i] * k;
		}
		return result;
	}

	const mat3 operator * (const mat3& m, const float k) {
		mat3 result;
		for (int i = 0; i < 9; i++) {
			result.elements[i] = m.elements[i] * k;
		}
		return result;
	}

	const vec3 operator * (const mat3& m, const vec3& v) {
		vec3 result;
		result.x = v.x * m.elements[0] + v.y * m.elements[0 + 1 * 3] + v.z * m.elements[0 + 2 * 3];
		result.y = v.x * m.elements[1 + 0 * 3] + v.y * m.elements[1 + 1 * 3] + v.z * m.elements[1 + 2 * 3];
		result.z = v.x * m.elements[2 + 0 * 3] + v.y * m.elements[2 + 1 * 3] + v.z * m.elements[2 + 2 * 3];
		return result;
	}

	const mat3 operator * (const mat3& m0, const mat3& m1) {
		mat3 result;
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				float sum = 0.0f;
				for (int e = 0; e < 3; e++) {
					sum += m0.elements[x + e * 3] * m1.elements[e + y * 3];
				}
				result.elements[x + y * 3] = sum;
			}
		}
		return result;
	}

	const bool operator == (const mat3& m0, const mat3& m1) {
		for (int i = 0; i < 9; i++) {
			float comp = m0.elements[i] - m1.elements[i];
			if (comp > DELTA) {
				return false;
			}
		}
		return true;
	}

	const bool operator != (const mat3& m0, const mat3& m1) {
		return !(m0 == m1);
	}

	std::ostream& operator << (std::ostream& os, const mat3& m) {
		//    0    3    6   
		//    1    4    7   
		//    2    5    8
		os << "[" << m.elements[0 + 0 *3] << "," << m.elements[0+1*3] << "," << m.elements[0 + 2 * 3] << "," << std::endl
			 << m.elements[1 + 0 * 3] << "," << m.elements[1 + 1 * 3] << "," << m.elements[1 + 2 * 3] << ","<< std::endl
		<< m.elements[2 + 0 * 3] << "," << m.elements[2 + 1 * 3] << "," << m.elements[2 + 2 * 3] << "]" << std::endl;

		return os;
	}
}