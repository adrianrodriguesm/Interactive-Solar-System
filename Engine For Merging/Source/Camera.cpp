#include "Camera.h"

Camera::Camera() {
	Eye = Center = Up = View = Right = vec3();
	ViewMatrix = ProjectionMatrix = mat4();
}

Camera::Camera(vec3 eye, vec3 center, vec3 up) {
	View = normalize(center-eye);
	Right = normalize(Cross(View, up));
	Up = normalize(Cross(Right, View));
	Eye = eye;
	Center = center;
	ViewMatrix = MatrixFactory::createViewMatrix(Eye, Center, Up);
}

void Camera::update() {
	ViewMatrix = MatrixFactory::createViewMatrix(Eye, Center, Up);
	View = normalize(Center - Eye);
	Right = normalize(Cross(View, Up));
	Up = normalize(Cross(Right, View));
}

void Camera::update(vec3 eye, vec3 center, vec3 up) {
	Eye = eye;
	Center = center;
	ViewMatrix = MatrixFactory::createViewMatrix(Eye, Center, up);
	View = normalize(center - eye);
	Right = normalize(Cross(View, Up));
	Up = normalize(Cross(Right, View));
}
