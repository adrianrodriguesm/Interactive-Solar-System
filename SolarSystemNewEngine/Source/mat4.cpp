#include "../Include/mat4.h"




mat4::mat4() {
	for (int i = 0; i < 16; i++) {
		this->elements[i] = 0.0f;
	}

	this->elements[0] = this->elements[1 + 1 * 4] = this->elements[2 + 2 * 4] = this->elements[3 + 3 * 4] = 1.0f;
}

mat4::mat4(float k) {
	for (int i = 0; i < 16; i++) {
		this->elements[i] = 0;
	}

	this->elements[0] = this->elements[1 + 1 * 4] = this->elements[2 + 2 * 4] = this->elements[3 + 3 * 4] = k;
}

mat4::mat4(const float m0, const float m1, const float m2, const float m3,
	const float m4, const float m5, const float m6, const float m7,
	const float m8, const float m9, const float m10, const float m11,
	const float m12, const float m13, const float m14, const float m15) {

	// 4x4 matrix - column major
	//    0    4    8  12
	//    1    5    9  13
	//    2    6    10 14
	//    3    7    11 15

	this->elements[0] = m0;
	this->elements[4] = m4;
	this->elements[8] = m8;
	this->elements[12] = m12;

	this->elements[1] = m1;
	this->elements[5] = m5;
	this->elements[9] = m9;
	this->elements[13] = m13;

	this->elements[2] = m2;
	this->elements[6] = m6;
	this->elements[10] = m10;
	this->elements[14] = m14;

	this->elements[3] = m3;
	this->elements[7] = m7;
	this->elements[11] = m11;
	this->elements[15] = m15;
}

mat4::mat4(const mat4& m) {
	for (int i = 0; i < 16; i++) {
		this->elements[i] = m.elements[i];
	}
}
mat4& mat4::operator = (const mat4& other) {
	for (int i = 0; i < 16; i++)
	{
		this->elements[i] = other.elements[i];
	}
	return *this;
}

const float determinant(const mat4& m) {
		
	float inv[16], det;
		

	inv[0] = m.elements[5] * m.elements[10] * m.elements[15] -
		m.elements[5] * m.elements[11] * m.elements[14] -
		m.elements[9] * m.elements[6] * m.elements[15] +
		m.elements[9] * m.elements[7] * m.elements[14] +
		m.elements[13] * m.elements[6] * m.elements[11] -
		m.elements[13] * m.elements[7] * m.elements[10];

	inv[4] = -m.elements[4] * m.elements[10] * m.elements[15] +
		m.elements[4] * m.elements[11] * m.elements[14] +
		m.elements[8] * m.elements[6] * m.elements[15] -
		m.elements[8] * m.elements[7] * m.elements[14] -
		m.elements[12] * m.elements[6] * m.elements[11] +
		m.elements[12] * m.elements[7] * m.elements[10];

	inv[8] = m.elements[4] * m.elements[9] * m.elements[15] -
		m.elements[4] * m.elements[11] * m.elements[13] -
		m.elements[8] * m.elements[5] * m.elements[15] +
		m.elements[8] * m.elements[7] * m.elements[13] +
		m.elements[12] * m.elements[5] * m.elements[11] -
		m.elements[12] * m.elements[7] * m.elements[9];

	inv[12] = -m.elements[4] * m.elements[9] * m.elements[14] +
		m.elements[4] * m.elements[10] * m.elements[13] +
		m.elements[8] * m.elements[5] * m.elements[14] -
		m.elements[8] * m.elements[6] * m.elements[13] -
		m.elements[12] * m.elements[5] * m.elements[10] +
		m.elements[12] * m.elements[6] * m.elements[9];

	inv[1] = -m.elements[1] * m.elements[10] * m.elements[15] +
		m.elements[1] * m.elements[11] * m.elements[14] +
		m.elements[9] * m.elements[2] * m.elements[15] -
		m.elements[9] * m.elements[3] * m.elements[14] -
		m.elements[13] * m.elements[2] * m.elements[11] +
		m.elements[13] * m.elements[3] * m.elements[10];

	inv[5] = m.elements[0] * m.elements[10] * m.elements[15] -
		m.elements[0] * m.elements[11] * m.elements[14] -
		m.elements[8] * m.elements[2] * m.elements[15] +
		m.elements[8] * m.elements[3] * m.elements[14] +
		m.elements[12] * m.elements[2] * m.elements[11] -
		m.elements[12] * m.elements[3] * m.elements[10];

	inv[9] = -m.elements[0] * m.elements[9] * m.elements[15] +
		m.elements[0] * m.elements[11] * m.elements[13] +
		m.elements[8] * m.elements[1] * m.elements[15] -
		m.elements[8] * m.elements[3] * m.elements[13] -
		m.elements[12] * m.elements[1] * m.elements[11] +
		m.elements[12] * m.elements[3] * m.elements[9];

	inv[13] = m.elements[0] * m.elements[9] * m.elements[14] -
		m.elements[0] * m.elements[10] * m.elements[13] -
		m.elements[8] * m.elements[1] * m.elements[14] +
		m.elements[8] * m.elements[2] * m.elements[13] +
		m.elements[12] * m.elements[1] * m.elements[10] -
		m.elements[12] * m.elements[2] * m.elements[9];

	inv[2] = m.elements[1] * m.elements[6] * m.elements[15] -
		m.elements[1] * m.elements[7] * m.elements[14] -
		m.elements[5] * m.elements[2] * m.elements[15] +
		m.elements[5] * m.elements[3] * m.elements[14] +
		m.elements[13] * m.elements[2] * m.elements[7] -
		m.elements[13] * m.elements[3] * m.elements[6];

	inv[6] = -m.elements[0] * m.elements[6] * m.elements[15] +
		m.elements[0] * m.elements[7] * m.elements[14] +
		m.elements[4] * m.elements[2] * m.elements[15] -
		m.elements[4] * m.elements[3] * m.elements[14] -
		m.elements[12] * m.elements[2] * m.elements[7] +
		m.elements[12] * m.elements[3] * m.elements[6];

	inv[10] = m.elements[0] * m.elements[5] * m.elements[15] -
		m.elements[0] * m.elements[7] * m.elements[13] -
		m.elements[4] * m.elements[1] * m.elements[15] +
		m.elements[4] * m.elements[3] * m.elements[13] +
		m.elements[12] * m.elements[1] * m.elements[7] -
		m.elements[12] * m.elements[3] * m.elements[5];

	inv[14] = -m.elements[0] * m.elements[5] * m.elements[14] +
		m.elements[0] * m.elements[6] * m.elements[13] +
		m.elements[4] * m.elements[1] * m.elements[14] -
		m.elements[4] * m.elements[2] * m.elements[13] -
		m.elements[12] * m.elements[1] * m.elements[6] +
		m.elements[12] * m.elements[2] * m.elements[5];

	inv[3] = -m.elements[1] * m.elements[6] * m.elements[11] +
		m.elements[1] * m.elements[7] * m.elements[10] +
		m.elements[5] * m.elements[2] * m.elements[11] -
		m.elements[5] * m.elements[3] * m.elements[10] -
		m.elements[9] * m.elements[2] * m.elements[7] +
		m.elements[9] * m.elements[3] * m.elements[6];

	inv[7] = m.elements[0] * m.elements[6] * m.elements[11] -
		m.elements[0] * m.elements[7] * m.elements[10] -
		m.elements[4] * m.elements[2] * m.elements[11] +
		m.elements[4] * m.elements[3] * m.elements[10] +
		m.elements[8] * m.elements[2] * m.elements[7] -
		m.elements[8] * m.elements[3] * m.elements[6];

	inv[11] = -m.elements[0] * m.elements[5] * m.elements[11] +
		m.elements[0] * m.elements[7] * m.elements[9] +
		m.elements[4] * m.elements[1] * m.elements[11] -
		m.elements[4] * m.elements[3] * m.elements[9] -
		m.elements[8] * m.elements[1] * m.elements[7] +
		m.elements[8] * m.elements[3] * m.elements[5];

	inv[15] = m.elements[0] * m.elements[5] * m.elements[10] -
		m.elements[0] * m.elements[6] * m.elements[9] -
		m.elements[4] * m.elements[1] * m.elements[10] +
		m.elements[4] * m.elements[2] * m.elements[9] +
		m.elements[8] * m.elements[1] * m.elements[6] -
		m.elements[8] * m.elements[2] * m.elements[5];

	det = m.elements[0] * inv[0] + m.elements[1] * inv[4] + m.elements[2] * inv[8] + m.elements[3] * inv[12];

	return det;
}
mat4 transpose(const mat4& m) {
	//    0    4    8  12
	//    1    5    9  13
	//    2    6    10 14
	//    3    7    11 15
	mat4 transpose;
	transpose.elements[0] = m.elements[0];
	transpose.elements[1] = m.elements[4];
	transpose.elements[2] = m.elements[8];
	transpose.elements[3] = m.elements[12];

	transpose.elements[4] = m.elements[1];
	transpose.elements[5] = m.elements[5];
	transpose.elements[6] = m.elements[9];
	transpose.elements[7] = m.elements[13];

	transpose.elements[8] = m.elements[2];
	transpose.elements[9] = m.elements[6];
	transpose.elements[10] = m.elements[10];
	transpose.elements[11] = m.elements[14];

	transpose.elements[12] = m.elements[3];
	transpose.elements[13] = m.elements[7];
	transpose.elements[14] = m.elements[11];
	transpose.elements[15] = m.elements[15];

	return transpose;

}

mat4 inverse(const mat4& m) {
	mat4 inverse;

	float inv[16], det;
	

	inv[0] = m.elements[5] * m.elements[10] * m.elements[15] -
		m.elements[5] * m.elements[11] * m.elements[14] -
		m.elements[9] * m.elements[6] * m.elements[15] +
		m.elements[9] * m.elements[7] * m.elements[14] +
		m.elements[13] * m.elements[6] * m.elements[11] -
		m.elements[13] * m.elements[7] * m.elements[10];

	inv[4] = -m.elements[4] * m.elements[10] * m.elements[15] +
		m.elements[4] * m.elements[11] * m.elements[14] +
		m.elements[8] * m.elements[6] * m.elements[15] -
		m.elements[8] * m.elements[7] * m.elements[14] -
		m.elements[12] * m.elements[6] * m.elements[11] +
		m.elements[12] * m.elements[7] * m.elements[10];

	inv[8] = m.elements[4] * m.elements[9] * m.elements[15] -
		m.elements[4] * m.elements[11] * m.elements[13] -
		m.elements[8] * m.elements[5] * m.elements[15] +
		m.elements[8] * m.elements[7] * m.elements[13] +
		m.elements[12] * m.elements[5] * m.elements[11] -
		m.elements[12] * m.elements[7] * m.elements[9];

	inv[12] = -m.elements[4] * m.elements[9] * m.elements[14] +
		m.elements[4] * m.elements[10] * m.elements[13] +
		m.elements[8] * m.elements[5] * m.elements[14] -
		m.elements[8] * m.elements[6] * m.elements[13] -
		m.elements[12] * m.elements[5] * m.elements[10] +
		m.elements[12] * m.elements[6] * m.elements[9];

	inv[1] = -m.elements[1] * m.elements[10] * m.elements[15] +
		m.elements[1] * m.elements[11] * m.elements[14] +
		m.elements[9] * m.elements[2] * m.elements[15] -
		m.elements[9] * m.elements[3] * m.elements[14] -
		m.elements[13] * m.elements[2] * m.elements[11] +
		m.elements[13] * m.elements[3] * m.elements[10];

	inv[5] = m.elements[0] * m.elements[10] * m.elements[15] -
		m.elements[0] * m.elements[11] * m.elements[14] -
		m.elements[8] * m.elements[2] * m.elements[15] +
		m.elements[8] * m.elements[3] * m.elements[14] +
		m.elements[12] * m.elements[2] * m.elements[11] -
		m.elements[12] * m.elements[3] * m.elements[10];

	inv[9] = -m.elements[0] * m.elements[9] * m.elements[15] +
		m.elements[0] * m.elements[11] * m.elements[13] +
		m.elements[8] * m.elements[1] * m.elements[15] -
		m.elements[8] * m.elements[3] * m.elements[13] -
		m.elements[12] * m.elements[1] * m.elements[11] +
		m.elements[12] * m.elements[3] * m.elements[9];

	inv[13] = m.elements[0] * m.elements[9] * m.elements[14] -
		m.elements[0] * m.elements[10] * m.elements[13] -
		m.elements[8] * m.elements[1] * m.elements[14] +
		m.elements[8] * m.elements[2] * m.elements[13] +
		m.elements[12] * m.elements[1] * m.elements[10] -
		m.elements[12] * m.elements[2] * m.elements[9];

	inv[2] = m.elements[1] * m.elements[6] * m.elements[15] -
		m.elements[1] * m.elements[7] * m.elements[14] -
		m.elements[5] * m.elements[2] * m.elements[15] +
		m.elements[5] * m.elements[3] * m.elements[14] +
		m.elements[13] * m.elements[2] * m.elements[7] -
		m.elements[13] * m.elements[3] * m.elements[6];

	inv[6] = -m.elements[0] * m.elements[6] * m.elements[15] +
		m.elements[0] * m.elements[7] * m.elements[14] +
		m.elements[4] * m.elements[2] * m.elements[15] -
		m.elements[4] * m.elements[3] * m.elements[14] -
		m.elements[12] * m.elements[2] * m.elements[7] +
		m.elements[12] * m.elements[3] * m.elements[6];

	inv[10] = m.elements[0] * m.elements[5] * m.elements[15] -
		m.elements[0] * m.elements[7] * m.elements[13] -
		m.elements[4] * m.elements[1] * m.elements[15] +
		m.elements[4] * m.elements[3] * m.elements[13] +
		m.elements[12] * m.elements[1] * m.elements[7] -
		m.elements[12] * m.elements[3] * m.elements[5];

	inv[14] = -m.elements[0] * m.elements[5] * m.elements[14] +
		m.elements[0] * m.elements[6] * m.elements[13] +
		m.elements[4] * m.elements[1] * m.elements[14] -
		m.elements[4] * m.elements[2] * m.elements[13] -
		m.elements[12] * m.elements[1] * m.elements[6] +
		m.elements[12] * m.elements[2] * m.elements[5];

	inv[3] = -m.elements[1] * m.elements[6] * m.elements[11] +
		m.elements[1] * m.elements[7] * m.elements[10] +
		m.elements[5] * m.elements[2] * m.elements[11] -
		m.elements[5] * m.elements[3] * m.elements[10] -
		m.elements[9] * m.elements[2] * m.elements[7] +
		m.elements[9] * m.elements[3] * m.elements[6];

	inv[7] = m.elements[0] * m.elements[6] * m.elements[11] -
		m.elements[0] * m.elements[7] * m.elements[10] -
		m.elements[4] * m.elements[2] * m.elements[11] +
		m.elements[4] * m.elements[3] * m.elements[10] +
		m.elements[8] * m.elements[2] * m.elements[7] -
		m.elements[8] * m.elements[3] * m.elements[6];

	inv[11] = -m.elements[0] * m.elements[5] * m.elements[11] +
		m.elements[0] * m.elements[7] * m.elements[9] +
		m.elements[4] * m.elements[1] * m.elements[11] -
		m.elements[4] * m.elements[3] * m.elements[9] -
		m.elements[8] * m.elements[1] * m.elements[7] +
		m.elements[8] * m.elements[3] * m.elements[5];

	inv[15] = m.elements[0] * m.elements[5] * m.elements[10] -
		m.elements[0] * m.elements[6] * m.elements[9] -
		m.elements[4] * m.elements[1] * m.elements[10] +
		m.elements[4] * m.elements[2] * m.elements[9] +
		m.elements[8] * m.elements[1] * m.elements[6] -
		m.elements[8] * m.elements[2] * m.elements[5];

	det = m.elements[0] * inv[0] + m.elements[1] * inv[4] + m.elements[2] * inv[8] + m.elements[3] * inv[12];

	if (det == 0.0f) {
	return false;
	}

	det = 1.0f / det;

	for (int i = 0; i < 16; i++) {
		inverse.elements[i] = inv[i] * det;
	}

	
	return inverse;
			
}

const mat4 operator + (const mat4& m0, const mat4& m1) {
	mat4 result;
	for (int i = 0; i < 16; i++) {
		result.elements[i] = m0.elements[i] + m1.elements[i];
	}
	return result;
		
}
const mat4 operator - (const mat4& m0, const mat4& m1) {
	mat4 result;
	for (int i = 0; i < 16; i++) {
		result.elements[i] = m0.elements[i] - m1.elements[i];
	}
	return result;
}

const mat4 operator * (const float k, const mat4& m) {
	mat4 result;
	for (int i = 0; i < 16; i++) {
		result.elements[i] = m.elements[i] * k;
	}
	return result;
}
const mat4 operator * (const mat4& m, const float k) {
	mat4 result;
	for (int i = 0; i < 16; i++) {
		result.elements[i] = m.elements[i] * k;
	}
	return result;
}

const mat4 operator * (const mat4& m0, const mat4& m1) {
	mat4 result;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			float sum = 0.0f;
			for (int e = 0; e < 4; e++) {
				sum += m0.elements[x + e * 4] * m1.elements[e + y * 4];
			}
			result.elements[x + y * 4] = sum;
		}
	}
	return result;
}

const vec4 operator * (const mat4& m, const vec4& v) {
	vec4 result;
	result.x = v.x * m.elements[0] + v.y * m.elements[0 + 1 * 4] + v.z * m.elements[0 + 2 * 4] + v.w * m.elements[0 + 3 * 4];
	result.y = v.x * m.elements[1 + 0 * 4] + v.y * m.elements[1 + 1 * 4] + v.z * m.elements[1 + 2 * 4] + v.w * m.elements[1 + 3 * 4];
	result.z = v.x * m.elements[2 + 0 * 4] + v.y * m.elements[2 + 1 * 4] + v.z * m.elements[2 + 2 * 4] + v.w * m.elements[2 + 3 * 4];
	result.w = v.x * m.elements[3 + 0 * 4] + v.y * m.elements[3 + 1 * 4] + v.z * m.elements[3 + 2 * 4] + v.w * m.elements[3 + 3 * 4];
	return result;
}

const bool operator == (const mat4& m0, const mat4& m1) {
	float DELTA = 0.001f;
	for (int i = 0; i < 16; i++) {
		float comp = m0.elements[i] - m1.elements[i];
		if (comp > DELTA) {
			return false;
		}
	}
	return true;
}

const bool operator != (const mat4& m0, const mat4& m1) {
	return !(m0 == m1);
}

const void mClean(mat4& m)
{
	for (int i = 0; i < 16; i++) {
		if (fabs(m.elements[i]) < mThreshold) m.elements[i] = 0.0f;
	}
}

std::ostream& operator << (std::ostream& os, const mat4& m) {
	os << "[" << m.elements[0] << "," << m.elements[4] << "," << m.elements[8] << "," << m.elements[12] << "," << std::endl
		<<  m.elements[1] << "," << m.elements[5] << "," << m.elements[9] << "," << m.elements[13] << "," << std::endl
		<< m.elements[2] << "," << m.elements[6] << "," << m.elements[10] << "," << m.elements[14] << ","  << std::endl
		<< m.elements[3] << "," << m.elements[7] << "," << m.elements[11] << "," << m.elements[15] <<  "]" << std::endl;

	return os;

}

