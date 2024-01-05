#ifndef CAMERA_H
#define CAMERA_H
#include "vec3.h"
#include "mat4.h"
#define _USE_MATH_DEFINES		// M_PI (on older platforms)
#include <math.h>
#include "cfg.h"
enum Dir {
	forward,
	left,
	right,
	back,
	up,
	down,
};
struct Camera {
	vec3 position = vec3(0, 0, 2);
	vec3 lookDir = vec3(0, 0, -1);
	vec3 up = vec3(0, 1, 0);
	vec3 target;
	mat4 cameraM4;

	void recalculate();
public:
	mat4 getViewMatrix();
	mat4 getProjectionMatrix(int windowWidth, int windowHeight);
	vec3 getPosition();
	void setPosition(vec3 newPosition);
	void rotateCamera(float angle, vec3 axis);
};
#endif // !CAMERA_H

