#include "dynamic_object.h"

DynamicObject::DynamicObject(Geometry* g, Material* m, float mass) : LeafObject(g, m), m(mass) {}

void DynamicObject::setCurrentNetForce(vec3 newForce) {
	currentNetForce = newForce;
}

float DynamicObject::getMass() {
	return m;
}

void DynamicObject::animate(float tstart, float tend) {
	const float dt = (tend - tstart) * PHYSICS::c;
	const vec3 v = (p / m); // velocity
	p = p + (currentNetForce - v * PHYSICS::rho) * dt; // update inertia
	position = position + v * dt; // update position
	modelChanged();
}