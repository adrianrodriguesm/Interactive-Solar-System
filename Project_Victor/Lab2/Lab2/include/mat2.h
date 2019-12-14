#include "vectors.h"
namespace mathsMat2
{

	struct mat2
	{
		float elements[2 * 2];
		
		mat2();
		mat2(const float k);
		mat2(const float m0, const float m1, const float m2, const float m3);
		mat2(const mat2& m);
		friend mat2 transpose(const mat2& m);
		mat2& operator = (const mat2& other);
		friend const float determinant(const mat2& m);
		friend mat2 inverse(const mat2& m);
		friend const mat2 operator + (const mat2& m0, const mat2& m1);
		friend const mat2 operator - (const mat2& m0, const mat2& m1);
		friend const mat2 operator * (const float k, const mat2& m);
		friend const mat2 operator * (const mat2& m, const float k);
		friend const vec2 operator * (const mat2& m, const vec2& v);
		friend const mat2 operator * (const mat2& m0, const mat2& m1);
		friend const bool operator == (const mat2& m0, const mat2& m1);
		friend const bool operator != (const mat2& m0, const mat2& m1);
		friend std::ostream& operator << (std::ostream& os, const mat2& m);
	};
}

