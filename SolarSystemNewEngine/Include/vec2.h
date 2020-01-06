#pragma once
#include <iostream>



//la otra opcion era hacerla class en vez 
//de struct la unica diferencia	es que esta es publica y la otra es privada
struct vec2 //usamos lowercase para tener consistencia con la utilizacion de matrizes en opengl
{
	float DELTA = 0.001f;
	float x, y;
	vec2();
	vec2(const float& k);
	vec2(const float& x, const float& y);


	//friend permite que miembros exclusivos accedan a estas fucniones	
	friend const vec2 normalize(const vec2& vector);
	friend const vec2 operator + (const vec2& left, const vec2& right);
	friend const vec2 operator - (const vec2& left, const vec2& right);
	friend const vec2 operator * (const vec2& left , const float right);
	friend const vec2 operator * (const float left, const vec2& right);
	friend const vec2 operator / (const vec2& left, const float right);

	//operaciones mas practicas
	vec2& operator+=(const vec2& other);
	vec2& operator-=(const vec2& other);
	vec2& operator*=(const float& scalar);
	bool operator==(const vec2& other);
	bool operator!=(const vec2& other);
	vec2& operator = (const vec2& other);

	friend std::ostream& operator << (std::ostream& stream, const vec2& vector);
	friend std::istream& operator >> (std::istream& stream, vec2& vector);
			
};



