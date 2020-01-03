#include <Vectors.h>


using namespace std;


						//----------------------------------- 2D -----------------------------------//

#pragma region 2D

vec2::vec2() {
	x = y = 0.f;
}

vec2::vec2(const float val1) {
	x = val1;
	y = 0.f;
}


vec2::vec2(const float val1, const float val2) {
	x = val1;
	y = val2;
}

vec2::vec2(const vec2& v) {
	x = v.x;
	y = v.y;
}

void vec2::clean() {
	this->x = 0.f;
	this->y = 0.f;
}

float vec2::ValByIndex(int i) const{
	switch (i) {
	case 0: return x;
	case 1: return y;
	default: return x;
	};
}

//Returns x by default
float vec2::ValByAxis(char c) const{
	switch (c) {
	case 'x': return x;
	case 'y': return y;
	default: return x;
	};
}

float vec2::Magnitude() const{
	return sqrt((this->x * this->x) + (this->y * this->y));
};

float vec2::MagnitudeSqrd() const{
	return (this->x * this->x) + (this->y * this->y);
};

const vec2 normalize(const vec2& v) {
	if (v.MagnitudeSqrd() == 0) return v;
	return vec2(v.x / v.Magnitude(), v.y / v.Magnitude());
};

const vec2 operator+ (const vec2 v1, const vec2 v2) {
	return vec2(v1.x + v2.x, v1.y + v2.y);
};

const vec2 operator-(const vec2 v1, const vec2 v2) {
	return vec2(v1.x - v2.x, v1.y - v2.y);;
};

const vec2 operator* (const vec2 v1, const float a) {
	return vec2(v1.x * a, v1.y * a);
};

const vec2 operator* (const float a, const vec2 v1) {
	return vec2(v1.x * a, v1.y * a);
};


const bool operator==(const vec2 v1, const vec2 v2) {
	if (v1.x == v2.x && v1.y == v2.y) { return true; }
	else { return false; }
};

const bool operator!=(const vec2 v1, const vec2 v2) {
	if (v1.x == v2.x && v1.y == v2.y) { return false; }
	else { return true; }
};

vec2& vec2::operator+= (const vec2& v2) {
	this->x += v2.x;
	this->y += v2.y;
	return *this;
};

vec2& vec2::operator-= (const vec2& v2) {
	this->x -= v2.x;
	this->y -= v2.y;
	return *this;
};

vec2& vec2::operator*= (const float a) {
	this->x *= a;
	this->y *= a;
	return *this;
};

const float Dot(const vec2 v1, const vec2 v2) {
	return (v1.x * v2.x) + (v1.y * v2.y);
}

const float CosAngle(const vec2 v1, const vec2 v2) {
	return (Dot(v1, v2)) / (v1.Magnitude() * v2.Magnitude());
}

const double Angle(const vec2 v1, const vec2 v2) {
	return acos((Dot(v1, v2)) / (v1.Magnitude() * v2.Magnitude()));
}

std::ostream& operator<<(std::ostream& os, const vec2 v) {
	os << "(" << std::to_string(v.x) << ", " << std::to_string(v.y) << ")" << std::endl;
	return os;
};

std::istream& operator>>(std::istream& is, vec2& v) {
	cout << "Type x-value: ";
	is >> v.x;
	cout << "Type y-value: ";
	is >> v.y;
	return is;
};

vec2 vec2::ProjectOn(vec2 v2) {
	float part = this->Magnitude() * CosAngle(*this, v2);
	return v2 * (part / v2.Magnitude());
}
#pragma endregion

						//----------------------------------- 3D -----------------------------------//

#pragma region 3D
vec3::vec3() {
	x = y = z = 0.f;
}

vec3::vec3(const float xval) {
	x = xval;
	y = 0.f;
	z = 0.f;
}

vec3::vec3(const float xval, const float yval) {
	x = xval;
	y = yval;
	z = 0.f;
}

vec3::vec3(const float xval, const float yval, const float zval) {
	x = xval;
	y = yval;
	z = zval;
}

vec3::vec3(const vec2 v) {
	x = v.x;
	y = v.y;
	z = 0.f;
}

vec3::vec3(const vec4 v) {
	x = v.x;
	y = v.y;
	z = v.z;
}


vec3::vec3(const vec3& v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

float vec3::Magnitude() const {
	return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
};

float vec3::MagnitudeSqrd() const {
	return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
};

const vec3 normalize(const vec3& v) {
	if (v.MagnitudeSqrd() == 0) return v;
	return (1/v.Magnitude())*v;
};

const vec3 operator+ (const vec3 v1, const vec3 v2) {
	return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
};

const vec3 operator- (const vec3 v1, const vec3 v2) {
	return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
};

const vec3 operator* (const vec3 v1, const float a) {
	return vec3(v1.x * a, v1.y * a, v1.z * a);
};

const vec3 operator* (const float a, const vec3 v1) {
	return vec3(v1.x * a, v1.y * a, v1.z * a);
};

const bool operator== (const vec3 v1, const vec3 v2) {
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z) { return true; }
	else { return false; }
};

const bool operator!= (const vec3 v1, const vec3 v2) {
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z) { return false; }
	else { return true; }
};

vec3& vec3::operator+= (const vec3& v2) {
	this->x += v2.x;
	this->y += v2.y;
	this->z += v2.z;
	return *this;
};

vec3& vec3::operator-= (const vec3& v2) {
	this->x -= v2.x;
	this->y -= v2.y;
	this->z -= v2.z;
	return *this;
};

vec3& vec3::operator*= (const float a) {
	this->x *= a;
	this->y *= a;
	this->z *= a;
	return *this;
};

const float Dot(const vec3 v1, const vec3 v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

//Calculating cross product using Cross Product Matrix Formula
const vec3 Cross(const vec3 v1, const vec3 v2) {
	return vec3(v1.y * v2.z - (v1.z * v2.y), 
		v1.z * v2.x - (v1.x * v2.z), 
		v1.x * v2.y - (v1.y * v2.x));
}

const float CosAngle(const vec3 v1, const vec3 v2) {
	return (Dot(v1, v2)) / (v1.Magnitude() * v2.Magnitude());
}

const double Angle(const vec3 v1, const vec3 v2) {
	return acos((Dot(v1, v2)) / (v1.Magnitude() * v2.Magnitude()));
}

const vec3 vecLerp(const vec3 vStart, const vec3 vEnd, const float amount) {
	return (vStart + amount * (vEnd - vStart));
}

std::ostream& operator<<(std::ostream& os, const vec3 v) {
	os << "(" << std::to_string(v.x) << ", " << std::to_string(v.y) << ", " << std::to_string(v.z) << ")" << std::endl;
	return os;
};

std::istream& operator>>(std::istream& is, vec3& v) {
	cout << "Type x-value: ";
	is >> v.x;
	cout << "Type y-value: ";
	is >> v.y;
	cout << "Type z-value: ";
	is >> v.z;
	return is;
};

vec2 vec3::Tovec2() {
	return vec2(this->x, this->y);
};

vec4 vec3::Tovec4() {
	return vec4(this->x, this->y, this->z, 0.0f);
};

#pragma endregion

						//----------------------------------- 4D -----------------------------------//

#pragma region 4D	
vec4::vec4() {
	x = y = z = w = 0.f;
}

vec4::vec4(float val1) {
	x = val1;
	y = val1;
	z = val1;
	w = 0.f;
}

vec4::vec4(float val1, float val2) {
	x = val1;
	y = val2;
	z = 0.f;
	w = 0.f;
}

vec4::vec4(float val1, float val2, float val3) {
	x = val1;
	y = val2;
	z = val3;
	w = 0.f;
}

vec4::vec4(float val1, float val2, float val3, float val4) {
	x = val1;
	y = val2;
	z = val3;
	w = val4;
}

vec4::vec4(const vec3 v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = 0.f;
}

vec4::vec4(const vec3 v, const float inw) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = inw;
}

vec4::vec4(const vec2 vec2) {
	x = vec2.x;
	y = vec2.y;
	z = 0.f;
	w = 0.f;
}

vec4::vec4(const vec4& vec4) {
	x = vec4.x;
	y = vec4.y;
	z = vec4.z;
	w = vec4.w;
}

float vec4::Magnitude() const {
	return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
};

float vec4::MagnitudeSqrd() const {
	return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
};

const vec4 normalize (const vec4& v) {
	if (v.MagnitudeSqrd() == 0) return v;
	return (1 / v.Magnitude())*v;
};

const vec4 operator+ (const vec4 v1, const vec4 v2) {
	return vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
};

const vec4 operator- (const vec4 v1, const vec4 v2) {
	return vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
};

const vec4 operator* (const vec4 v1, const float a) {
	return vec4(v1.x * a, v1.y * a, v1.z * a, v1.w * a);
};

const vec4 operator* (const float a, const vec4 v1) {
	return vec4(v1.x * a, v1.y * a, v1.z * a, v1.w * a);
};

const bool operator== (const vec4 v1, const vec4 v2) {
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w) { return true; }
	else { return false; }
};

const bool operator!= (const vec4 v1, const vec4 v2) {
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w) { return false; }
	else { return true; }
};

vec4& vec4::operator+= (const vec4& v2) {
	this->x += v2.x;
	this->y += v2.y;
	this->z += v2.z;
	this->w += v2.w;
	return *this;
};

vec4& vec4::operator-= (const vec4& v2) {
	this->x -= v2.x;
	this->y -= v2.y;
	this->z -= v2.z;
	this->w -= v2.w;
	return *this;
};

vec4& vec4::operator*= (const float a) {
	this->x *= a;
	this->y *= a;
	this->z *= a;
	this->w *= a;
	return *this;
};

const float Dot(const vec4 v1, const vec4 v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}

const float Dot3D(const vec4 v1, const vec4 v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

//Calculating cross product using Cross Product Matrix Formula
const vec4 Cross(const vec4 v1, const vec4 v2) {
	return vec4(v1.y * v2.z - (v1.z * v2.y), v1.z * v2.x - (v1.x * v2.z), v1.x * v2.y - (v1.y * v2.x));
}

const float CosAngle(const vec4 v1, const vec4 v2) {
	return (Dot3D(v1, v2)) / (v1.Magnitude() * v2.Magnitude());
}

const double Angle(const vec4 v1, const vec4 v2) {
	return acos((Dot3D(v1, v2)) / (v1.Magnitude() * v2.Magnitude()));
}

std::ostream& operator<<(std::ostream& os, const vec4 v) {
	os << "(" << std::to_string(v.x) << ", " << std::to_string(v.y) << ", " << std::to_string(v.z) << ", " << std::to_string(v.w) << ")" << std::endl;
	return os;
};

std::istream& operator>>(std::istream& is, vec4& v) {
	cout << "Type x-value: ";
	is >> v.x;
	cout << "Type y-value: ";
	is >> v.y;
	cout << "Type z-value: ";
	is >> v.z;
	cout << "Type w-value: ";
	is >> v.w;
	return is;
};

vec3 vec4::Tovec3() {
	return vec3(this->x, this->y, this->z);
};

#pragma endregion

