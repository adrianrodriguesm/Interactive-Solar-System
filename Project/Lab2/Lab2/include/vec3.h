#pragma once

#include <iostream>

namespace mathsVec3
{

		//la otra opcion era hacerla class en vez 
		//de struct la unica diferencia	es que esta es publica y la otra es privada
		struct vec3 //usamos lowercase para tener consistencia con la utilizacion de matrizes en opengl
		{
			float DELTA = 0.0001f;
			float x, y, z;
			vec3();
			vec3(const float& k);
			vec3(const float& x, const float& y);
			vec3(const float& x, const float& y, const float& z);

			//Methods
			const float dotProduct(const vec3& other) const;
			const vec3 crossProduct(const vec3& other);//Me devuelve una copia
			

			
			friend const vec3 normalize(const vec3& vector);
			friend const vec3 operator + (const vec3& left, const vec3& right);
			friend const vec3 operator - (const vec3& left, const vec3& right);
			friend const vec3 operator * (const vec3& left, const float right);
			friend const vec3 operator * (const float left, const vec3& right);
			friend const vec3 operator / (const vec3& left, const float right);
			

			//operaciones mas practicas
			vec3& operator+=(const vec3& other);
			vec3& operator-=(const vec3& other);
			vec3& operator*=(const float& scalar);
			bool operator==(const vec3& other);
			bool operator!=(const vec3& other);
			vec3& operator = (const vec3& other);

			//friend permite que miembros exclusivos accedan a estas fucniones
			friend std::ostream& operator<<(std::ostream& stream, const vec3& vector);
			friend std::istream& operator >> (std::istream& stream, vec3& vector);


			
		};

}

