#ifndef UTIL_H
#define UTIL_H
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

struct Material {
	vec3 kd, ks, ka;
	float shininess;

	Material() { ka = vec3(1, 1, 1), kd = ks = vec3(0, 0, 0); shininess = 20; }
};
struct Light {
	vec3 La, Le;
	vec4 wLightPos;
};

struct RenderState {
	mat4 MVP, M, Minv, V, P, lightVP;
	Material* material;
	Light light;
	vec3 wEye;
};
struct VertexData {
	vec3 position, normal, color = vec3(1, 1, 1);
};
struct Ray {
	vec3 start, dir;
	Ray(vec3 _start, vec3 _dir) { start = _start; dir = normalize(_dir); }
};


inline float getRandom() {
	return float(rand()) / float((RAND_MAX));
}

#endif // !UTIL_H