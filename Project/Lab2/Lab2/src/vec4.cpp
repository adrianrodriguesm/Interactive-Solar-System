#include "../include/vec4.h"

namespace mathsVec4
{
		vec4::vec4()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}

		vec4::vec4(const float& x, const float& y, const float& z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = 1.0f;
		}


		vec4::vec4(const float& x, const float& y, const float& z, const float& w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}



		/*
		const vec4 operator + (const vec4& left, const vec4& right)
		{

			return vec4(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
		}

		const vec4 operator - (const vec4& left, const vec4& right)
		{
			return vec4(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
		}

		//Foi feito desta maneira para não alterar a refencia
		const vec4 operator * (const vec4& left, const float right)
		{
			return vec4(left.x * right, left.y * right, left.z * right, left.w);
		}


		const vec4 operator * (const float left, const vec4& right)
		{
			return vec4(right.x * left, right.y * left, right.z * left, right.w );
		}
		
		const vec4 operator / (const vec4& left, const float right)
		{
			return vec4(left.x / right, left.y / right, left.z / right, left.w);
		}
		*/
		const vec4 normalize(const vec4& v)
		{
			vec4 vn;
			float s = 1 / (v.w * sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
			vn.x = v.x * s;
			vn.y = v.y * s;
			vn.z = v.z * s;
			vn.w = 1.0f;
			return vn;
		}
		
		bool vec4::operator == (const vec4& other)
		{
			if ((x - other.x) <= DELTA && (y - other.y) <= DELTA && (z - other.z) <= DELTA && (w - other.w) <= DELTA) {
				return true;
			}
			return false;
		}

		bool vec4::operator != (const vec4& other) 
		{
			return !(*this == other);
		}


		vec4& vec4::operator = (const vec4& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}


		//como un metodo toString
		std::ostream& operator<<(std::ostream& stream, const vec4& vector)
		{
			stream << "Vector 4D : (" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
			return stream;
		}

		std::istream& operator >> (std::istream& stream, vec4& vector)
		{

			stream >> vector.x >> vector.y >> vector.z >> vector.w;
			return stream;
		}


	}
