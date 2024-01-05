#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "vec3.h"

struct Triangle {
	vec3 vertices[3];
	vec3 normal;

	Triangle(vec3 p0, vec3 p1, vec3 p2) {
		vertices[0] = p0;
		vertices[1] = p1;
		vertices[2] = p2;

		normal = normalize(cross(p1 - p0, p2 - p0));
	}
};
#endif // !TRIANGLE_H

