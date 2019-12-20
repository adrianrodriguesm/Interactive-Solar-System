#pragma once

#include <iostream>

namespace mathsVec4
{

		//la otra opcion era hacerla class en vez 
		//de struct la unica diferencia	es que esta es publica y la otra es privada
		struct vec4 //usamos lowercase para tener consistencia con la utilizacion de matrizes en opengl
		{
			float DELTA = 0.001f;
			float x, y, z, w;
			vec4();
			//SE USA CONST PORQUE NO SE QUIERE CAMBIAR
			vec4(const float& x, const float& y, const float& z);
			vec4(const float& x, const float& y, const float& z, const float& w);			

			//uno es normal porque es ahi donde vamos a guardar las alteraciones
			friend const vec4 normalize(const vec4& vector);
			
			friend const vec4 operator + (const vec4& left, const vec4& right);
			friend const vec4 operator - (const vec4& left, const vec4& right);
			friend const vec4 operator * (const vec4& left, const float right);
			friend const vec4 operator * (const float left, const vec4& right);
			friend const vec4 operator / (const vec4& left, const float right);
			

			//operaciones mas practicas
			vec4& operator+=(const vec4& other);
			vec4& operator-=(const vec4& other);
			vec4& operator*=(const float& scalar); 
			
			bool operator==(const vec4& other);
			bool operator!=(const vec4& other);
			vec4& operator = (const vec4& other);

			//friend permite que miembros exclusivos accedan a estas fucniones
			friend std::ostream& operator<<(std::ostream& stream, const vec4& vector);
			friend std::istream& operator >> (std::istream& stream, vec4& vector);


			
		};

}

