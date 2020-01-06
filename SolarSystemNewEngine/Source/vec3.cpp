#include "../Include/vec3.h"


vec3::vec3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

vec3::vec3(const float& x, const float& y, const float& z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

vec3::vec3(const float& x, const float& y)
{
	this->x = x;
	this->y = y;
	this->z = 0.0f;
}

vec3::vec3(const float& k)
{
	this->x = k;
	this->y = k;
	this->z = k;
}
const vec3 normalize(const vec3& vector)
{
	float x2 = pow(vector.x, 2);
	float y2 = pow(vector.y, 2);
	float z2 = pow(vector.z, 2);
	float magnitude = sqrt((x2 + y2 + z2));
	return vec3(vector.x / magnitude, vector.y / magnitude, vector.z / magnitude);
}

const float vec3::dotProduct(const vec3& other) const
{
	return  (x * other.x + y * other.y + z * other.z);
}

const vec3 vec3::crossProduct(const vec3& other) 
{
	float xCross = (0 * other.x) + ((-z) * other.y) + (y * other.z);
	float yCross = (z * other.x) + (0 * other.y) + ((-x) * other.z);
	float zCross = ((-y) * other.x) + (x * other.y) + (0 * other.z);
			
	return vec3(y * other.z-z * other.y, 
		z*other.x-x*other.z, 
		x*other.y - y *other.x);

}


const vec3 operator + (const vec3& left, const vec3& right)
{
	return vec3(left.x + right.x, left.y + right.y, left.z + right.z);
}

const vec3 operator - (const vec3& left, const vec3& right)
{
	return vec3(left.x - right.x, left.y - right.y, left.z - right.z);
}

//Foi feito desta maneira para não alterar a refencia
const vec3 operator * (const vec3& left, const float right)
{
	return vec3(left.x * right, left.y * right, left.z * right);
}


const vec3 operator * (const float left, const vec3& right)
{
	return vec3(right.x * left, right.y * left, right.z * left);
}
		
const vec3 operator / (const vec3& left, const float right) 
{
	return vec3(left.x / right, left.y / right, left.z / right);
}

		
		
bool vec3::operator==(const vec3& other) 
{
	if (std::abs(x - other.x) <= DELTA && std::abs(y - other.y) <= DELTA && std::abs(z - other.z) <= DELTA) {
		return true;
	}
	return false;
}

bool vec3::operator!=(const vec3& other)
{
	return !(*this == other);
}

//Como nos es friend hay hacer referenci ve2::
vec3& vec3::operator+=(const vec3& other) 
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

vec3& vec3::operator-=(const vec3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

vec3& vec3::operator*=(const float& scalar) 
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

vec3& vec3::operator = (const vec3& other) 
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}


//como un metodo toString
std::ostream& operator<<(std::ostream& stream, const vec3& vector)
{
	stream << "Vector 3D : (" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	return stream;
}

std::istream& operator >> (std::istream& stream, vec3& vector)
{

	stream >> vector.x >> vector.y >> vector.z;
	return stream;
}

