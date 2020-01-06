#include "../Include/vec2.h"

vec2::vec2()
{
	x = 0.0f;
	y = 0.0f;
}

vec2::vec2(const float& x, const float& y) 
{
	this->x = x;
	this->y = y;
}

vec2::vec2(const float& k)
{
	this->x = k;
	this->y = k;
}

const vec2 normalize(const vec2& vector) 
{
	float x2 = pow(vector.x, 2);
	float y2 = pow(vector.y, 2);
	float magnitude = sqrt((x2 + y2));
	return vec2(vector.x / magnitude, vector.y / magnitude);
}

const vec2 operator + (const vec2& left, const vec2& right)
{
	vec2 result = vec2(left.x + right.x, left.y + right.y);
	return result;
}

const vec2 operator - (const vec2& left, const vec2& right)
{
	vec2 result = vec2(left.x - right.x, left.y - right.y);
	return result;
}

//Foi feito desta maneira para não alterar a refencia
const vec2 operator * (const vec2& left, const float right) {
	return vec2(left.x * right, left.y * right);
}


const vec2 operator * (const float left, const vec2& right) {
	return vec2(right.x * left, right.y * left );
}
		
const vec2 operator / (const vec2& left, const float right) {
	return vec2(left.x / right, left.y / right);
}
		
		
bool vec2::operator==(const vec2& other) {
			
	if ((x - other.x) <= DELTA && (y - other.y) <= DELTA) {
		return true;
	}
	return false;
}

bool vec2::operator!=(const vec2& other) {
	return !(*this == other);
}

//Como nos es friend hay hacer referenci ve2::
vec2& vec2::operator+=(const vec2& other) {
	x += other.x;
	y += other.y;
	return *this;
			
}

vec2& vec2::operator-=(const vec2& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

vec2& vec2::operator*=(const float& scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

vec2& vec2::operator = (const vec2& other)
{
	x = other.x;
	y = other.y;
	return *this;
}


//como un metodo toString
std::ostream& operator << (std::ostream& stream, const vec2& vector)
{
	stream << "Vector 2D : (" << vector.x << ", " << vector.y << ")";
	return stream;
}

std::istream& operator >> (std::istream & stream, vec2& vector)
{
		
	stream >> vector.x >> vector.y;
	return stream;
}



