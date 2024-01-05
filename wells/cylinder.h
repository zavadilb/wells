#ifndef CILINDER_H
#define CILINDER_H

#include "param_surface.h"

struct Cylinder : public ParamSurface {
	float radius;
	float height;
	vec3 color;

	inline Cylinder(float _radius, float _height, vec3 _color) : radius(_radius), height(_height), color(_color) {
		int nU = 1; 
		int nV = (int)(_radius * 60.0f); // "eyeballed" the constants
		create(nU, nV, false, true);
	}
	inline VertexData genVertexData(float u, float v) {
		VertexData vd;

		// y is up!
		vd.position.x = radius * cosf(u * 2.0f * M_PI);
		vd.position.z = radius * sinf(u * 2.0f * M_PI);
		vd.position.y = height * v;

		vd.normal = normalize(vec3(vd.position.x, 0, vd.position.z));

		vd.color = color;
		return vd;
	}
};

#endif // !CILINDER_H

