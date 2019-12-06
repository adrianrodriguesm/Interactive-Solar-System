#include "../include/vectors.h"
#include "../include/mat4.h"
#include "../include/mat3.h"
#include "../include/mat2.h"
using  namespace mathsMat4;
using  namespace mathsMat3;
using  namespace mathsMat2;

#define M_PI       3.14159265358979323846f
namespace mathsMatFactory
{
	struct matFactory
	{
		
		static const float DegToRad(float degrees)
		{
			return (float)(degrees * (M_PI / 180.0f));
		};
		static const mat2 createZeroMat2();
		static const mat2 createIdentityMat2();
		static const mat2 createScaleMat2(const vec2& v);
		static const mat2 createRotationMat2(const float angle);

		static const mat3 createZeroMat3();
		static const mat3 createIdentityMat3();
		static const mat3 createScaleMat3(const vec3& v);
		static const mat3 createRotationMat3(const float angle, const vec3& v);
		static const mat3 createDualMat3(const vec3& v);
		static const mat3 createMat3FromMat4(const mat4& m);


		static const mat4 createZeroMat4();
		static const mat4 createIdentityMat4();
		static const mat4 createScaleMat4(const vec3& v);
		static const mat4 createRotationMat4(const float angle, const vec3& v);
		static const mat4 createTranslationMat4(const vec3& v);


		static const mat4 createOrthographipProjectionMatrix(const float left,
			const float right, const float bottom, const float top, const float near,
			const float far);
		static const mat4 createPerspectiveProjectionMatrix(const float fov, const float aspectRatio,
			const float near,const float far);
		static const mat4 createLookAtMatrix(const vec3& eye, const vec3& direction, const vec3& up);
		
	};
}

