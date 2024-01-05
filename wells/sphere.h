#ifndef SPHERE_H
#define SPHERE_H
#include "param_surface.h"
#include "vec3.h"
#define _USE_MATH_DEFINES		// M_PI (on older platforms)
#include <math.h>
struct Sphere : public ParamSurface {
public:
	float radius;
	vec3 color;

	inline Sphere(float r, vec3 c = vec3(1, 1, 1)) : radius(r), color(c) { 
		int nUV = (int) (r * 60); // "eyeballed" the constant
		create(nUV, nUV); 
	}
	inline VertexData genVertexData(float u, float v) {
		VertexData vd;
		vd.position = vd.normal = radius * vec3(cosf(u * 2.0f * (float)M_PI) * sinf(v * (float)M_PI),
			sinf(u * 2.0f * (float)M_PI) * sinf(v * (float)M_PI),
			cosf(v * (float)M_PI));
		vd.color = color;
		return vd;
	}
};
#endif // !SPHERE_H