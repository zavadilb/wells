#ifndef CONE_H
#define CONE_H

#include "param_surface.h"

struct Cone : public ParamSurface {
	float angle;
	float height;
	vec3 color;

	inline Cone(float _angle, float _height, vec3 _color) : angle(_angle), height(_height), color(_color) {
		const int nU = 6; // Does not depend on size. Should be 1 but then streaks are visible (for some reason)
		const float baseCircleRadius = tanf(_angle * 0.5f) * _height;
		const int nV = (int)(baseCircleRadius * 60); // "eyeballed" the constant
		create(nU, nV, false, true);
	}
	inline VertexData genVertexData(float u, float v) {
		VertexData vd;

		// y is up!

		// Construct a <height> tall \/ cone with <angle> angle, where y is pointing up utilizing parametric rotation around the +y axis.
		const float halfAngle = 0.5f * angle;
		vd.position.x = v * height * sinf(halfAngle) * cosf(u * 2.0f * M_PI);
		vd.position.z = v * height * sinf(halfAngle) * sinf(u * 2.0f * M_PI);
		vd.position.y = v * height;

		// Tip is at (0,0,0), meaning rotation of the cone with a matrix without translation will point it in a given direction

		// Parametric surface => normal can be calculated this way
		// Mathematical optimization could probably be done here.

		// when v==0, the triangles with a vertex at the very tip of the cone have shading normals of length 0,
		// because the lhs of the cross product is 0 (because v is 0). The end result of this is that at the very tip
		// of the cone we have a whole strip, that gets no (only ambient) lighting (because a vector of length 0 is used in the light calculation).
		// To avoid this, v is set to 1 at these edge cases, which yields a surprisingly realistic result.
		float optimizedv = v != 0 ? v : 1.0f;
		vd.normal = -1.0f * cross(
			vec3( // (d/du) * f(u,v)
				-2.0f * M_PI * optimizedv * sinf(halfAngle) * sinf(2.0f * M_PI * u),
				0.0f,
				2.0f * M_PI * optimizedv * sinf(halfAngle) * cosf(2.0f * M_PI * u)
				), 
			vec3( // (d/dv) * f(u,v)
				sinf(halfAngle) * cosf(u * 2.0f * M_PI),
				cosf(halfAngle),
				sinf(halfAngle) * sinf(u * 2.0f * M_PI)
			)  
		);

		vd.color = color;

		vd.normal = normalize(vd.normal);

		return vd;
	}
};

#endif // !CONE_H

/*
	A tribute to the ray-tracing implementation of a cone in another project.
	I will never forget the suffering you have caused me.
*/

//class Cone : public Intersectable {
//protected:
//	vec3 tip;
//	vec3 axis;
//	float angle;
//	float height;
//
//public:
//	Cone(vec3 _tip, vec3 _axis, float _angle, float _height, Material* m) : tip(_tip), angle(_angle), height(_height) {
//		axis = normalize(_axis);
//		material = m;
//	}
//
//	Hit intersect(const Ray& ray) { // using the equasion of the cone
//		const float cas = cosf(angle) * cosf(angle);
//		const vec3 q = ray.start - tip;
//
//		const float a = -1.0f * cas * dot(ray.dir, ray.dir) + dot(ray.dir, axis) * dot(ray.dir, axis);
//		const float b = 2.0f * dot(ray.dir, axis) * dot(q, axis) - 2.0f * cas * dot(ray.dir, q);
//		const float c = dot(q, axis) * dot(q, axis) - cas * dot(q, q);
//
//		Hit hit;
//		float disc = b * b - (4.0f * a * c);
//		if (disc < 0) return hit; // no real solutions
//
//		const float t1 = (-1.0f * b + sqrt(disc)) / (2.0f * a);
//		const float t2 = (-1.0f * b - sqrt(disc)) / (2.0f * a);
//
//		float t = (t2 < t1) ? t2 : t1;
//		vec3 position = ray.start + ray.dir * t;
//		float hitHeight = dot((position - tip), axis);
//
//		if ((hitHeight < 0 || hitHeight > height) /*hit on conic but not cone*/ || t < 0 /*hit behind eye*/) {
//			t = (t2 < t1) ? t1 : t2; // check other hit
//			position = ray.start + ray.dir * t;
//			float hitHeight = dot((position - tip), axis);
//			if ((hitHeight < 0 || hitHeight > height) /*hit on conic but not cone*/ || t < 0 /*hit behind eye*/) {
//				return hit; //on the conic but not on the cone
//			}
//		}
//
//		hit.t = t;
//		hit.material = material;
//		hit.position = position;
//		vec3 p_t = normalize(hit.position - tip);
//		hit.normal = 2.0f * (dot(p_t, axis)) * axis - 2.0f * (p_t)*cas;
//		return hit;
//	}
//
//};