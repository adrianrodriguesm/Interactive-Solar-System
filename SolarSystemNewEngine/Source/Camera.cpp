#include "../Include/Camera.h"



Camera::Camera() {
	this->d = 0.0f;
	this->viewMatrix = matFactory::createTranslationMat4(vec3(0, 0, -d));
	this->projMatrix = matFactory::createIdentityMat4();
		
	this->qRot = qtrn(1.0f, 0.0f, 0.0f, 0.0f);
		
}

Camera::Camera(const vec3& eye, const vec3& direction, const vec3& up) {
	this->viewMatrix = matFactory::createLookAtMatrix(eye, direction, up);
	this->projMatrix = matFactory::createIdentityMat4();
		
	this->qRot = qtrn(1.0f, 0.0f, 0.0f, 0.0f);
	this->d = eye.z;
}

void Camera::setPersProjMatrix(const float fov, const float aspectRatio, const float near, const float far) {
	this->projMatrix = matFactory::createPerspectiveProjectionMatrix(fov, aspectRatio, near, far);
}

void Camera::setOrthoProjMatrix(const float left, const float right, const float bottom, const float top,
	const float near, const float far) {
	this->projMatrix = matFactory::createOrthographipProjectionMatrix(left, right, bottom, top, near, far);
}

void Camera::setRotMatrix(float xOffset, float yOffset) {
	qtrn qpitch1 = qtrn::qFromAngleAxis(yOffset, axis_x);
	qtrn qyaw1 = qtrn::qFromAngleAxis(xOffset, axis_y);
	qRot = (qpitch1 * qyaw1) * qRot;
	mat4 rotMatrix;
	qGLMatrix(qRot, rotMatrix);
	viewMatrix = matFactory::createTranslationMat4(vec3(0, 0, -d)) * rotMatrix;
}

void Camera::setTranslationMatrix(const float d) {
	this->d = d;
	mat4 rotMatrix;
	qGLMatrix(qRot, rotMatrix);
	viewMatrix = matFactory::createTranslationMat4(vec3(0, 0, -d)) * rotMatrix;
}
