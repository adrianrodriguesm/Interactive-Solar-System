#include <Matrices.h>


						//------------------ 2x2 MATRIX -----------------//	

mat2::mat2() {
	data[0] = data[1] = data[2] = data[3] = 0;
}

mat2::mat2(const float a) {
	data[0] = data[3] = a ;
	data[1] = data[2] = 0;
}

mat2::mat2(const float m0, const float m1,
	const float m2, const float m3) {
	data[0] = m0;
	data[1] = m1;
	data[2] = m2;
	data[3] = m3;
}

mat2::mat2(const mat2& m) {
	data[0] = m.data[0];
	data[1] = m.data[1];
	data[2] = m.data[2];
	data[3] = m.data[3];
}

void mat2::clean() {
}

float mat2::determinant() const {
	return (data[0] * data[3] - data[1] * data[2]);
}

mat2 transpose(const mat2& m) {
	return mat2(m.data[0], m.data[2], m.data[1], m.data[3]);
}

mat2 inverse(const mat2& m) {
	if (m.determinant() == 0) {
		cout << "The matrix is not inversible : Determinant is zero." << endl;
		return m;
	}
	return (1/m.determinant())*mat2(m.data[3], -m.data[1], -m.data[2], m.data[0]);
}

mat2& mat2::operator=(const mat2& m) {
	for (int i = 0; i < 4;i++) {
		this->data[i] = m.data[i];
	}
	return *this;
}

mat2& mat2::operator+=(const mat2& m) {
	for (int i = 0; i < 4;i++) {
		this->data[i] += m.data[i];
	}
	return *this;
}

mat2& mat2::operator-=(const mat2& m) {
	for (int i = 0; i < 4;i++) {
		this->data[i] -= m.data[i];
	}
	return *this;
}

const bool operator==(const mat2& m0, const mat2& m1) {
	for (int i = 0; i < 4;i++) {
		if (fabs(m0.data[i] - m1.data[i]) > EPSILON) return false;
	}
	return true;
}

const bool operator!=(const mat2& m0, const mat2& m1) {
	for (int i = 0; i < 4;i++) {
		if (m0.data[i] != m1.data[i]) return true;
	}
	return false;
}

const mat2 operator+(const mat2& m0, const mat2& m1) {
	mat2 m = mat2();
	for (int i = 0; i < 4;i++) {
		m.data[i]= m0.data[i] + m1.data[i];
	}
	return m;
}

const mat2 operator-(const mat2& m0, const mat2& m1) {
	mat2 m = mat2();
	for (int i = 0; i < 4;i++) {
		m.data[i] = m0.data[i] - m1.data[i];
	}
	return m;
}

const mat2 operator*(const mat2& m0, const mat2& m1) {
	return mat2(m0.data[0] * m1.data[0] + m0.data[1] * m1.data[2], m0.data[0] * m1.data[1] + m0.data[1] * m1.data[3],
				m0.data[2] * m1.data[0] + m0.data[3] * m1.data[2], m0.data[2] * m1.data[1] + m0.data[3] * m1.data[3]);
}

const mat2 operator*(const mat2& m0, const float a) {
	return mat2(m0.data[0] *a, m0.data[1] * a, m0.data[2] * a, m0.data[3] * a);
}

const mat2 operator*(const float a, const mat2& m0) {
	return mat2(m0.data[0] * a, m0.data[1] * a, m0.data[2] * a, m0.data[3] * a);
}

const vec2 operator*(const mat2& m, const vec2& v) {
	return vec2(m.data[0] * v.x + m.data[1] * v.y,
				m.data[2] * v.x + m.data[3] * v.y);
}

std::ostream& operator<<(std::ostream& os, const mat2 m) {
	os << "|" << std::to_string(m.data[0]) << "  " << std::to_string(m.data[1]) << "|" << std::endl 
		<< "|" << std::to_string(m.data[2]) << "  " << std::to_string(m.data[3]) << "|" << std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, mat2& m) {
	cout << "Element(1,1): ";
	is >> m.data[0];
	cout << "Element(1,2): ";
	is >> m.data[1];
	cout << "Element(2,1): ";
	is >> m.data[2];
	cout << "Element(2,2): ";
	is >> m.data[3];
	return is;
}



						//------------------ 3x3 MATRIX -----------------//




mat3::mat3() {
	data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = data[6] = data[7] = data[8] = 0;
}

mat3::mat3(const float a) {
	data[0] = data[4] = data[8] = a;
	data[1] = data[2] = data[3] = data[5] = data[6] = data[7] = 0;
}

mat3::mat3(const float m0, const float m1, const float m2,
	const float m3, const float m4, const float m5,
	const float m6, const float m7, const float m8) {
	data[0] = m0;
	data[1] = m1;
	data[2] = m2;
	data[3] = m3;
	data[4] = m4;
	data[5] = m5;
	data[6] = m6;
	data[7] = m7;
	data[8] = m8;
}

mat3::mat3(const vec3& v1, const vec3& v2, const vec3& v3) {
	data[0] = v1.x;
	data[1] = v1.y;
	data[2] = v1.z;
	data[3] = v2.x;
	data[4] = v2.y;
	data[5] = v2.z;
	data[6] = v3.x;
	data[7] = v3.y;
	data[8] = v3.z;
}

mat3::mat3(const mat3& m) {
	data[0] = m.data[0];
	data[1] = m.data[1];
	data[2] = m.data[2];
	data[3] = m.data[3];
	data[4] = m.data[4];
	data[5] = m.data[5];
	data[6] = m.data[6];
	data[7] = m.data[7];
	data[8] = m.data[8];
}

void mat3::clean() {
	data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = data[6] = data[7] = data[8] = 0;
}

float mat3::determinant() const {
	return data[0] * (data[4] * data[8] - data[5] * data[7]) 
		- data[1] * (data[3] * data[8] - data[5] * data[6]) 
		+ data[2] * (data[3] * data[7] - data[4] * data[6]);
}

mat3 transpose(const mat3& m) {
	return mat3(m.data[0], m.data[3], m.data[6], 
				m.data[1], m.data[4], m.data[7],
				m.data[2], m.data[5], m.data[8]);
}

mat3 inverse(const mat3& m) {
	if (m.determinant() == 0) {
		cout << "The matrix is not inversible : Determinant is zero." << endl;
		return m;
	}
	//Matrix of determinants of minor matrices of mT (Matrix of cofactors):
	mat3 m0 = mat3(mat2(m.data[4], m.data[7], m.data[5], m.data[8]).determinant(), -mat2(m.data[1], m.data[7], m.data[2], m.data[8]).determinant(), mat2(m.data[1], m.data[4], m.data[2], m.data[5]).determinant(),
					-mat2(m.data[3], m.data[6], m.data[5], m.data[8]).determinant(), mat2(m.data[0], m.data[6], m.data[2], m.data[8]).determinant(), -mat2(m.data[0], m.data[3], m.data[2], m.data[5]).determinant(),
					mat2(m.data[3], m.data[6], m.data[4], m.data[7]).determinant(), -mat2(m.data[0], m.data[6], m.data[1], m.data[7]).determinant(), mat2(m.data[0], m.data[3], m.data[1], m.data[4]).determinant());
	return (1 / m.determinant() * m0);
}

mat3& mat3::operator=(const mat3& m) {
	for (int i = 0; i < 9; i++) {
		this->data[i] = m.data[i];
	}
	return *this;
}

mat3& mat3::operator+=(const mat3& m) {
	for (int i = 0; i < 9; i++) {
		this->data[i] += m.data[i];
	}
	return *this;
}

mat3& mat3::operator-=(const mat3& m) {
	for (int i = 0; i < 9; i++) {
		this->data[i] -= m.data[i];
	}
	return *this;
}

const bool operator==(const mat3& m0, const mat3& m1) {
	for (int i = 0; i < 9; i++) {
		if (fabs(m0.data[i] - m1.data[i]) > EPSILON) return false;
	}
	return true;
}

const bool operator!=(const mat3& m0, const mat3& m1) {
	for (int i = 0; i < 9; i++) {
		if (fabs(m0.data[i] - m1.data[i]) > EPSILON) return true;
	}
	return false;
}

const mat3 operator+(const mat3& m0, const mat3& m1) {
	mat3 m = mat3();
	for (int i = 0; i < 9; i++) {
		m.data[i] = m0.data[i] + m1.data[i];
	}
	return m;
}

const mat3 operator-(const mat3& m0, const mat3& m1) {
	mat3 m = mat3();
	for (int i = 0; i < 9; i++) {
		m.data[i] = m0.data[i] - m1.data[i];
	}
	return m;
}

const mat3 operator*(const mat3& m0, const mat3& m1) {
	return mat3((m0.data[0] * m1.data[0] + m0.data[1] * m1.data[3] + m0.data[2] * m1.data[6]), (m0.data[0] * m1.data[1] + m0.data[1] * m1.data[4] + m0.data[2] * m1.data[7]), (m0.data[0] * m1.data[2] + m0.data[1] * m1.data[5] + m0.data[2] * m1.data[8]),
				(m0.data[3] * m1.data[0] + m0.data[4] * m1.data[3] + m0.data[5] * m1.data[6]), (m0.data[3] * m1.data[1] + m0.data[4] * m1.data[4] + m0.data[5] * m1.data[7]), (m0.data[3] * m1.data[2] + m0.data[4] * m1.data[5] + m0.data[5] * m1.data[8]), 
				(m0.data[6] * m1.data[0] + m0.data[7] * m1.data[3] + m0.data[8] * m1.data[6]), (m0.data[6] * m1.data[1] + m0.data[7] * m1.data[4] + m0.data[8] * m1.data[7]), (m0.data[6] * m1.data[2] + m0.data[7] * m1.data[5] + m0.data[8] * m1.data[8]));
}

const mat3 operator*(const mat3& m0, const float a) {
	return mat3(m0.data[0] * a, m0.data[1] * a, m0.data[2] * a, 
				m0.data[3] * a, m0.data[4] * a, m0.data[5] * a, 
				m0.data[6] * a, m0.data[7] * a, m0.data[8] * a);
}

const mat3 operator*(const float a, const mat3& m0) {
	return mat3(m0.data[0] * a, m0.data[1] * a, m0.data[2] * a,
		m0.data[3] * a, m0.data[4] * a, m0.data[5] * a,
		m0.data[6] * a, m0.data[7] * a, m0.data[8] * a);
}

const vec3 operator*(const mat3& m, const vec3& v) {
	return vec3(m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z,
				m.data[3] * v.x + m.data[4] * v.y + m.data[5] * v.z,
				m.data[6] * v.x + m.data[7] * v.y + m.data[8] * v.z);
}

std::ostream& operator<<(std::ostream& os, const mat3 m) {
	os << "|" << std::to_string(m.data[0]) << "  " << std::to_string(m.data[1]) << "  " << std::to_string(m.data[2]) << "|" << std::endl
		<< "|" << std::to_string(m.data[3]) << "  " << std::to_string(m.data[4]) << "  " << std::to_string(m.data[5]) << "|" << std::endl
		<< "|" << std::to_string(m.data[6]) << "  " << std::to_string(m.data[7]) << "  " << std::to_string(m.data[8]) << "|" << std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, mat3& m) {
	cout << "Element(1,1): ";
	is >> m.data[0];
	cout << "Element(1,2): ";
	is >> m.data[1];
	cout << "Element(1,3): ";
	is >> m.data[2];
	cout << "Element(2,1): ";
	is >> m.data[3];
	cout << "Element(2,2): ";
	is >> m.data[4];
	cout << "Element(2,3): ";
	is >> m.data[5];
	cout << "Element(3,1): ";
	is >> m.data[6];
	cout << "Element(3,2): ";
	is >> m.data[7];
	cout << "Element(3,3): ";
	is >> m.data[8];
	return is;
}



								//------------------ 4x4 MATRIX -----------------//




mat4::mat4() {
	data[0] = data[1] = data[2] = data[3] = 
	data[4] = data[5] = data[6] = data[7] = 
	data[8] = data[9] = data[10] = data[11] = 
	data[12] = data[13] = data[14] = data[15] = 0;
}

mat4::mat4(const float a) {
	data[1] = data[2] = data[3] =
	data[4] = data[6] = data[7] =
	data[8] = data[9] = data[11] =
	data[12] = data[13] = data[14] = 0;
	data[0] = data[5] = data[10] = data[15] = a;
}

mat4::mat4(const float m0, const float m1, const float m2, const float m3,
	const float m4, const float m5, const float m6, const float m7,
	const float m8, const float m9, const float m10, const float m11,
	const float m12, const float m13, const float m14, const float m15) {
	data[0] = m0;
	data[1] = m1;
	data[2] = m2;
	data[3] = m3;
	data[4] = m4;
	data[5] = m5;
	data[6] = m6;
	data[7] = m7;
	data[8] = m8;
	data[9] = m9;
	data[10] = m10;
	data[11] = m11;
	data[12] = m12;
	data[13] = m13;
	data[14] = m14;
	data[15] = m15;
}

mat4::mat4(const mat4& m) {
	data[0] = m.data[0];
	data[1] = m.data[1];
	data[2] = m.data[2];
	data[3] = m.data[3];
	data[4] = m.data[4];
	data[5] = m.data[5];
	data[6] = m.data[6];
	data[7] = m.data[7];
	data[8] = m.data[8];
	data[9] = m.data[9];
	data[10] = m.data[10];
	data[11] = m.data[11];
	data[12] = m.data[12];
	data[13] = m.data[13];
	data[14] = m.data[14];
	data[15] = m.data[15];
}

void mat4::clean() {
	data[0] = data[1] = data[2] = data[3] =
	data[4] = data[5] = data[6] = data[7] =
	data[8] = data[9] = data[10] = data[11] =
	data[12] = data[13] = data[14] = data[15] = 0;
}

float mat4::determinant() const {
	return data[0] * mat3(data[5], data[6], data[7], 
						data[9], data[10], data[11], 
						data[13], data[14], data[15]).determinant()

		- data[4] * mat3(data[1], data[2], data[3],
						data[9], data[10], data[11],
						data[13], data[14], data[15]).determinant()

		+ data[8] * mat3(data[1], data[2], data[3],
						data[5], data[6], data[7],
						data[13], data[14], data[15]).determinant()

		- data[12] * mat3(data[1], data[2], data[3],
						data[5], data[6], data[7],
						data[9], data[10], data[11]).determinant();
}

mat4 transpose(const mat4& m) {
	return mat4(m.data[0], m.data[4], m.data[8], m.data[12],
				m.data[1], m.data[5], m.data[9], m.data[13],
				m.data[2], m.data[6], m.data[10], m.data[14], 
				m.data[3], m.data[7], m.data[11], m.data[15]);
}

mat4& mat4::operator=(const mat4& m) {
	for (int i = 0; i < 16; i++) {
		this->data[i] = m.data[i];
	}
	return *this;
}

mat4& mat4::operator+=(const mat4& m) {
	for (int i = 0; i < 16; i++) {
		this->data[i] += m.data[i];
	}
	return *this;
}

mat4& mat4::operator-=(const mat4& m) {
	for (int i = 0; i < 16; i++) {
		this->data[i] -= m.data[i];
	}
	return *this;
}

const bool operator==(const mat4& m0, const mat4& m1) {
	for (int i = 0; i < 16; i++) {
		if (fabs(m0.data[i] - m1.data[i]) > EPSILON) return false;
	}
	return true;
}

const bool operator!=(const mat4& m0, const mat4& m1) {
	for (int i = 0; i < 16; i++) {
		if (fabs(m0.data[i] - m1.data[i]) > EPSILON) return true;
	}
	return false;
}

const mat4 operator+(const mat4& m0, const mat4& m1) {
	mat4 m = mat4();
	for (int i = 0; i < 16; i++) {
		m.data[i] = m0.data[i] + m1.data[i];
	}
	return m;
}

const mat4 operator-(const mat4& m0, const mat4& m1) {
	mat4 m = mat4();
	for (int i = 0; i < 16; i++) {
		m.data[i] = m0.data[i] - m1.data[i];
	}
	return m;
}

const mat4 operator*(const mat4& m0, const mat4& m1) {
	return mat4((m0.data[0] * m1.data[0] + m0.data[1] * m1.data[4] + m0.data[2] * m1.data[8] + m0.data[3] * m1.data[12]), (m0.data[0] * m1.data[1] + m0.data[1] * m1.data[5] + m0.data[2] * m1.data[9] + m0.data[3] * m1.data[13]), (m0.data[0] * m1.data[2] + m0.data[1] * m1.data[6] + m0.data[2] * m1.data[10] + m0.data[3] * m1.data[14]), (m0.data[0] * m1.data[3] + m0.data[1] * m1.data[7] + m0.data[2] * m1.data[11] + m0.data[3] * m1.data[15]),
				(m0.data[4] * m1.data[0] + m0.data[5] * m1.data[4] + m0.data[6] * m1.data[8] + m0.data[7] * m1.data[12]), (m0.data[4] * m1.data[1] + m0.data[5] * m1.data[5] + m0.data[6] * m1.data[9] + m0.data[7] * m1.data[13]), (m0.data[4] * m1.data[2] + m0.data[5] * m1.data[6] + m0.data[6] * m1.data[10] + m0.data[7] * m1.data[14]), (m0.data[4] * m1.data[3] + m0.data[5] * m1.data[7] + m0.data[6] * m1.data[11] + m0.data[7] * m1.data[15]),
				(m0.data[8] * m1.data[0] + m0.data[9] * m1.data[4] + m0.data[10] * m1.data[8] + m0.data[11] * m1.data[12]), (m0.data[8] * m1.data[1] + m0.data[9] * m1.data[5] + m0.data[10] * m1.data[9] + m0.data[11] * m1.data[13]), (m0.data[8] * m1.data[2] + m0.data[9] * m1.data[6] + m0.data[10] * m1.data[10] + m0.data[11] * m1.data[14]), (m0.data[8] * m1.data[3] + m0.data[9] * m1.data[7] + m0.data[10] * m1.data[11] + m0.data[11] * m1.data[15]),
				(m0.data[12] * m1.data[0] + m0.data[13] * m1.data[4] + m0.data[14] * m1.data[8] + m0.data[15] * m1.data[12]), (m0.data[12] * m1.data[1] + m0.data[13] * m1.data[5] + m0.data[14] * m1.data[9] + m0.data[15] * m1.data[13]), (m0.data[12] * m1.data[2] + m0.data[13] * m1.data[6] + m0.data[14] * m1.data[10] + m0.data[15] * m1.data[14]), (m0.data[12] * m1.data[3] + m0.data[13] * m1.data[7] + m0.data[14] * m1.data[11] + m0.data[15] * m1.data[15]));
}

const mat4 operator*(const mat4& m0, const float a) {
	return mat4(m0.data[0] * a, m0.data[1] * a, m0.data[2] * a, m0.data[3] * a,
		m0.data[4] * a, m0.data[5] * a, m0.data[6] * a, m0.data[7] * a,
		m0.data[8] * a, m0.data[9] * a, m0.data[10] * a, m0.data[11] * a,
		m0.data[12] * a, m0.data[13] * a, m0.data[14] * a, m0.data[15] * a);
}

const mat4 operator*(const float a, const mat4& m0) {
	return mat4(m0.data[0] * a, m0.data[1] * a, m0.data[2] * a, m0.data[3] * a, 
				m0.data[4] * a, m0.data[5] * a, m0.data[6] * a, m0.data[7] * a, 
				m0.data[8] * a, m0.data[9] * a, m0.data[10] * a, m0.data[11] * a,
				m0.data[12] * a, m0.data[13] * a, m0.data[14] * a, m0.data[15] * a);
}

const vec4 operator*(const mat4& m, const vec4& v) {
	return vec4(m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z + m.data[3] * v.w,
				m.data[4] * v.x + m.data[5] * v.y + m.data[6] * v.z + m.data[7] * v.w,
				m.data[8] * v.x + m.data[9] * v.y + m.data[10] * v.z + m.data[11] * v.w,
				m.data[12] * v.x + m.data[13] * v.y + m.data[14] * v.z + m.data[15] * v.w);
}

std::ostream& operator<<(std::ostream& os, const mat4 m) {
	os << "|" << std::to_string(m.data[0]) << "  " << std::to_string(m.data[1]) << "  " << std::to_string(m.data[2]) << "  " << std::to_string(m.data[3]) << "|" << std::endl
		<< "|" << std::to_string(m.data[4]) << "  " << std::to_string(m.data[5]) << "  " << std::to_string(m.data[6]) << "  " << std::to_string(m.data[7]) << "|" << std::endl
		<< "|" << std::to_string(m.data[8]) << "  " << std::to_string(m.data[9]) << "  " << std::to_string(m.data[10]) << "  " << std::to_string(m.data[11]) << "|" << std::endl
		<< "|" << std::to_string(m.data[12]) << "  " << std::to_string(m.data[13]) << "  " << std::to_string(m.data[14]) << "  " << std::to_string(m.data[15]) << "|" << std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, mat4& m) {
	cout << "Element(1,1): ";
	is >> m.data[0];
	cout << "Element(1,2): ";
	is >> m.data[1];
	cout << "Element(1,3): ";
	is >> m.data[2];
	cout << "Element(1,4): ";
	is >> m.data[3];

	cout << "Element(2,1): ";
	is >> m.data[4];
	cout << "Element(2,2): ";
	is >> m.data[5];
	cout << "Element(2,3): ";
	is >> m.data[6];
	cout << "Element(2,4): ";
	is >> m.data[7];

	cout << "Element(3,1): ";
	is >> m.data[8];
	cout << "Element(3,2): ";
	is >> m.data[9];
	cout << "Element(3,3): ";
	is >> m.data[10];
	cout << "Element(3,4): ";
	is >> m.data[11];

	cout << "Element(4,1): ";
	is >> m.data[12];
	cout << "Element(4,2): ";
	is >> m.data[13];
	cout << "Element(4,3): ";
	is >> m.data[14];
	cout << "Element(4,4): ";
	is >> m.data[15];
	return is;
}

								//------------------ MATRIX FACTORY -----------------//



const mat2 MatrixFactory::createZeroMat2() {
	return mat2();
}

const mat2 MatrixFactory::createIdentityMat2() {
	return mat2(1);
}

const mat2 MatrixFactory::createScaleMat2(const vec2& v) {
	return mat2(v.x, 0, 0, v.y);
}

const mat2 MatrixFactory::createRoationMat2(const float angle) {
	return mat2(cos(angle), -sin(angle), 
				sin(angle), cos(angle));
}

const mat3 MatrixFactory::createZeroMat3() {
	return mat3();
}

const mat3 MatrixFactory::createIdentityMat3() {
	return mat3(1);
}

const mat3 MatrixFactory::createDualMat3(const vec3& v) {
	return mat3(0, -v.z, v.y, 
				v.z, 0, -v.x, 
				-v.y, v.x, 0);
}

const mat3 MatrixFactory::createScaleMat3(const vec3& v) {
	return mat3(v.x, 0, 0, 0, v.y, 0, 0, 0, v.z);
}

const mat3 MatrixFactory::createRoationMat3(const float angle, const vec3& axis) {
	vec3 a = normalize(axis);
	mat3 m = createDualMat3(axis);
	return (mat3(1) + sin(angle) * m + (1 - cos(angle)) * m * m);

}

const mat3 MatrixFactory::createMat3FromMat4(const mat4& m) {
	return mat3(m.data[0], m.data[1], m.data[2],
				m.data[4], m.data[5], m.data[6],
				m.data[8], m.data[9], m.data[10]);
}

const mat3 MatrixFactory::createMat3FromMat2(const mat2& m) {
	return mat3(m.data[0], m.data[1], 0,
		m.data[2], m.data[3],0,
		0,0,1);
}

const mat4 MatrixFactory::createZeroMat4() {
	return mat4();
}

const mat4 MatrixFactory::createIdentityMat4() {
	return mat4(1);
}

const mat4 MatrixFactory::createDualMat4(const vec3& v) {
	return mat4(0, -v.z, v.y, 0,
				v.z, 0, -v.x, 0,
				-v.y, v.x, 0, 0,
				0, 0, 0, 1);
}

const mat4 MatrixFactory::createScaleMat4(const vec3& v) {
	return mat4(v.x, 0, 0, 0,
				0, v.y, 0, 0,
				0, 0, v.z, 0,
				0, 0,   0, 1);
}

const mat4 MatrixFactory::createRoationMat4(const float ang, const vec3& axis) {
	vec3 a = normalize(axis);
	mat3 m = createDualMat3(a);
	float angle = ang *  DEGREES_TO_RADIANS;
	m = mat3(1) + sin(angle) * m + (1 - cos(angle)) * m * m;
	return mat4(m.data[0], m.data[1], m.data[2], 0,
		m.data[3], m.data[4], m.data[5], 0,
		m.data[6], m.data[7], m.data[8], 0,
		0, 0, 0, 1);
}

const mat4 MatrixFactory::createTranslationMat4(const vec3& v) {
	mat4 m = mat4(1);
	m.data[3] = v.x;
	m.data[7] = v.y;
	m.data[11] = v.z;
	return m;
}


const mat4 MatrixFactory::createMat4FromMat3(const mat3& m) {
	return mat4(m.data[0], m.data[1], m.data[2], 0,
		m.data[3], m.data[4], m.data[5], 0,
		m.data[6], m.data[7], m.data[8], 0,
		0, 0, 0, 1);
}

const mat4 MatrixFactory::createViewMatrix(const vec3 eye, const vec3 center, const vec3 up) {
	vec3 v = normalize(center - eye);
	vec3 u = up;
	vec3 s = Cross(v, u);
	u = Cross(s, v);
	u = normalize(u);
	s = normalize(s);
	mat4 M = createMat4FromMat3(mat3(s, u, -1 * v));
	M.data[3] = -1 * Dot(s, eye);
	M.data[7] = -1 * Dot(u, eye);
	M.data[11] = Dot(v, eye);
	return M;
}

const mat4 MatrixFactory::createOrthoProjectionMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far) {
	return mat4(2 / (right - left), 0, 0, (left + right) / (left - right),
				0, 2 / (top - bottom), 0, (bottom + top) / (bottom - top),
				0, 0, 2 / (near - far), (near + far) / (near - far),
				0, 0, 0, 1);
}

const mat4 MatrixFactory::createPerspectiveProjectionMatrix(const float ang, const float aspect, const float nearZ, const float farZ) {
	float fovY = ang * DEGREES_TO_RADIANS;
	float d = 1 / tan(fovY / 2);
	return mat4(d/aspect, 0.0f, 0.0f, 0.0f,
				0.0f, d, 0.0f, 0.0f,
				0.0f, 0.0f, (nearZ+farZ)/(nearZ-farZ), (2*farZ*nearZ)/(nearZ-farZ),
				0.0f, 0.0f, -1.0f, 0.0f);
}