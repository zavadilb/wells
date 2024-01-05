#ifndef WELL_H
#define WELL_H
#include "leaf_object.h"
#include "sphere.h"
class Well : public LeafObject
{
	float boundingSphereRadius = 0; // ray intersection based on bounding sphere
	float m = 35.1 * pow(10,9); // mass in kg (~mass of Itokawa asteroid https://en.wikipedia.org/wiki/25143_Itokawa)
public:
	Well(Geometry* g, Material* m, vec3 initPosition) : LeafObject(g, m) {
		translate(initPosition);

		// find bounding sphere radius 
		std::vector<VertexData>* const vertices = geometry->getVertices();
		for (size_t i = 0; i < vertices->size(); i++)
		{
			const float vtxDistance = length((*wPositions)[i] - position);
			if (vtxDistance > boundingSphereRadius) boundingSphereRadius = vtxDistance;
		}
	}
	const float getMass() {
		return m;
	}
	void setMass(float newMass) {
		m = newMass;
	}

	// ray - bounding sphere intersection. Returns negative value if there is no intersection
	float Intersect(const Ray& ray) {
		vec3 dist = ray.start - position;
		float a = dot(ray.dir, ray.dir);
		float b = dot(dist, ray.dir) * 2.0f;
		float c = dot(dist, dist) - boundingSphereRadius * boundingSphereRadius;
		float discr = b * b - 4.0f * a * c;
		if (discr < 0) return -1;
		float sqrt_discr = sqrtf(discr);
		float t1 = (-b + sqrt_discr) / 2.0f / a;	// t1 >= t2 for sure
		float t2 = (-b - sqrt_discr) / 2.0f / a;
		if (t1 <= 0) return -1;
		return (t2 > 0) ? t2 : t1;
	}

};
#endif // !WELL_H
