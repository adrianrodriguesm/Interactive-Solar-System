#include "../include/matFactory.h"
#include <math.h>


namespace mathsMatFactory
{


	const mat2 matFactory::createZeroMat2() {
		return mat2(0.0f);
	}
	const mat2 matFactory::createIdentityMat2() {
		return mat2();
	}
	const mat2 matFactory::createScaleMat2(const vec2& v) {
		mat2 scale = createIdentityMat2();
		scale.elements[0] = v.x;
		scale.elements[3] = v.y;
		return scale;
	}

	const mat2 matFactory::createRotationMat2(const float angle) {
		mat2 rot = createIdentityMat2();
		float r = DegToRad(angle);
		float c = cos(r);
		float s = sin(r);
		rot.elements[0]=c;
		rot.elements[1] = s;
		rot.elements[2] = -s;
		rot.elements[3] = c;
		return rot;
	}

	const mat3 matFactory::createZeroMat3() {
		return mat3(0.0f);
	}

	const mat3 matFactory::createIdentityMat3() {
		return mat3();
	}
	const mat3 matFactory::createScaleMat3(const vec3& v) {
		mat3 scale = createIdentityMat3();
		scale.elements[0 + 0 * 3] = v.x;
		scale.elements[1 + 1 * 3] = v.y;
		scale.elements[2 + 2 * 3] = v.z;
		return scale;
	}
	
	const mat3 matFactory::createRotationMat3(const float angle,const  vec3& v) {
		float rad = DegToRad(angle);
		mat3 identityMatrix = createIdentityMat3();
		vec3 normalV = normalize(v);
		mat3 dualMatrix = createDualMat3(normalV);
		mat3 sinM = sin(rad) * dualMatrix;
		mat3 cosM = (1 - cos(rad)) * dualMatrix * dualMatrix;
		return (identityMatrix + sinM + cosM);
		
	}

	const mat3 matFactory::createMat3FromMat4(const mat4& m) {
		mat3 result = createIdentityMat3();
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				result.elements[x + y * 3] = m.elements[x + y * 4];
			}
		}
		return result;
	}

	const mat3 matFactory::createDualMat3(const vec3& v) {	
		return mat3(0.0f, v.z, -v.y, -v.z,0.0f,v.x,v.y,-v.x,0.0f);
	}

	const mat4 matFactory::createZeroMat4() {
		return mat4(0.0f);
	}
	const mat4 matFactory::createIdentityMat4() {
		return mat4();
	}

	const mat4 matFactory::createTranslationMat4(const vec3& v) {
		mat4 result = createIdentityMat4();
		result.elements[0 + 3 * 4] = v.x;
		result.elements[1 + 3 * 4] = v.y;
		result.elements[2 + 3 * 4] = v.z;
		return result;
	}

	const mat4 matFactory::createScaleMat4(const vec3& v) {
		mat4 result = createIdentityMat4();
		result.elements[0 + 0 * 4] = v.x;
		result.elements[1 + 1 * 4] = v.y;
		result.elements[2 + 2 * 4] = v.z;
		return result;
	}

	const mat4 matFactory::createRotationMat4(const float angle, const vec3& v) {

		mat3 rotRodrigues = createRotationMat3(angle,v);
		mat4 result = createIdentityMat4();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.elements[i + j * 4] = rotRodrigues.elements[i + j * 3];
			}
		}

		return result;

	}

	const mat4 matFactory::createOrthographipProjectionMatrix(const float left,
		const float right, const float bottom, const float top, const float near,
		const float far) {

		mat4 result = createIdentityMat4();
		result.elements[0 * 4 + 0] = 2.0f / (right - left);
		result.elements[1 * 4 + 1] = 2.0f / (top - bottom);
		result.elements[2 * 4 + 2] = -2.0f / (far - near);
		result.elements[3 * 4 + 0] = (right + left) / (left - right);
		result.elements[3 * 4 + 1] = (top + bottom) / (bottom- top);
		result.elements[3 * 4 + 2] = (far + near) / (near - far);
		return result;

	}
	const mat4 matFactory::createPerspectiveProjectionMatrix(const float fov, const float aspectRatio,
		const float near, const float far) {
		mat4 result = createIdentityMat4();
		float f = 1.0f / tan(fov * (M_PI / 360.0f));
		result.elements[0] = f / aspectRatio;
		result.elements[1 * 4 + 1] = f;
		result.elements[2 * 4 + 2] = (far + near) / (near - far);
		result.elements[3 * 4 + 2] = (2.0f * far * near) / (near - far);
		result.elements[2 * 4 + 3] = -1.0f;
		result.elements[3 * 4 + 3] = 0.0f;
		return result;
	}

	const mat4 matFactory::createLookAtMatrix(const vec3& eye, const vec3& direction, const vec3& up) {

		vec3 camDirection = normalize(direction - eye);//n	
		vec3 camRight = normalize(camDirection.crossProduct(up));//u
		vec3 cameraUp = camRight.crossProduct(camDirection);
		mat4 viewMatrix = createIdentityMat4();

		viewMatrix.elements[0] = camRight.x;
		viewMatrix.elements[0 + 1 * 4] = camRight.y;
		viewMatrix.elements[0 + 2 * 4] = camRight.z;
		viewMatrix.elements[0 + 3 * 4] = -(eye.dotProduct(camRight));
		viewMatrix.elements[1 + 0 * 4] = cameraUp.x;
		viewMatrix.elements[1 + 1 * 4] = cameraUp.y;
		viewMatrix.elements[1 + 2 * 4] = cameraUp.z;
		viewMatrix.elements[1 + 3 * 4] = -(eye.dotProduct(cameraUp));
		viewMatrix.elements[2 + 0 * 4] = -camDirection.x;
		viewMatrix.elements[2 + 1 * 4] = -camDirection.y;
		viewMatrix.elements[2 + 2 * 4] = -camDirection.z;
		viewMatrix.elements[2 + 3 * 4] = (eye.dotProduct(camDirection));
		return viewMatrix;

	}


	
	
}