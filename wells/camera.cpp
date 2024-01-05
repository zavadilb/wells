#include "camera.h"
void Camera::recalculate() {
	target = position + lookDir;
	cameraM4 = PointAtMatrix(position, target, up);
}
mat4 Camera::getViewMatrix() {
	recalculate();
	return quickInverse(cameraM4);
}
mat4 Camera::getProjectionMatrix(int windowWidth, int windowHeight) {
	const float fov = M_PI / 4; // fov of 90 degrees
	const float fp = 0.01f; // front plane
	const float bp = 1000.0f;// back plane

	return ProjectionMatrix(fov, fp, bp, windowWidth, windowHeight);
}
vec3 Camera::getPosition() {
	return position;
}
void Camera::setPosition(vec3 newPosition) {
	position = newPosition;
}
void Camera::rotateCamera(float angle, vec3 axis) {
	lookDir = applyTransform(lookDir, RotationMatrix(angle, axis));
}