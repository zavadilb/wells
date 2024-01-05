#include "body.h"
Body::Body( Material* _material, vec3 ropeFixedAt, float _m) : 
	LeafObject(new CuboidMesh(vec3(0, 0, 0), vec3(0.15, 0.3, 0.15), vec3(123, 104, 238)) , _material), m(_m) 
{
	//TODO this is very ugly here
	bodyMesh = static_cast<CuboidMesh*>(geometry); // position relative to this->position
	s = ropeFixedAt;
	calcRopeFixationPoint();
	calcMomentOfInertia();
}

void Body::calcRopeFixationPoint() {
	l = position + applyTransform(vec3(0, length(bodyMesh->compY) * size.y * -0.5, 0), RotationMatrix(orientation)); // calculate new attachment point
}
void Body::calcMomentOfInertia() {
	const float compX = length(bodyMesh->compX) * size.x;
	const float compY = length(bodyMesh->compY) * size.y;
	I = m * (compX * compX + compY * compY) / 12; // moment of inertia (special case)
}


void Body::animate(float tstart, float tend) {
	const float dt = (tend - tstart) * PHYSICS::c;
	const vec3 K = length(l - s) >= l0 ? D * normalize((s - l)) * (length(s - l) - l0) : vec3(0, 0, 0); // Spring (rope) force
	const vec3 drag = PHYSICS::rho * (p / m);
	const vec3 F = m * PHYSICS::g + K - drag; // Sum of all forces
	const vec3 omega = L / I; // Special case (I is a scalar)
	vec3 M = cross(l - position, K) - PHYSICS::kappa * omega; // torque

	p = p + F * dt; // inertia
	position = position + (p / m) * dt;
	orientation = orientation + omega * dt;
	L = L + M * dt;

	calcRopeFixationPoint();
	modelChanged();
}

void Body::translate(vec3 translationVector) {
	GameObject::translate(translationVector);
	calcRopeFixationPoint();
}
void Body::setPosition(vec3 newPosition) {
	GameObject::setPosition(newPosition);
	calcRopeFixationPoint();
}
void Body::rotate(vec3 xyz) {
	GameObject::rotate(xyz);
	calcRopeFixationPoint();
}
void Body::setOrientation(vec3 xyz) {
	GameObject::setOrientation(xyz);
	calcRopeFixationPoint();
}
void Body::scale(vec3 scaleVector) {
	GameObject::scale(scaleVector);
	calcRopeFixationPoint();
	calcMomentOfInertia(); // moment of inertia also changed, because dimensions changed
}
void Body::setScale(vec3 scaleVector) {
	GameObject::scale(scaleVector);
	calcRopeFixationPoint();
	calcMomentOfInertia(); // moment of inertia also changed, because dimensions changed
}

void Body::setVelocity(vec3 v) {
	p = v * m;
}

Body::~Body() {
	if (material) delete material;
	if (bodyMesh) delete bodyMesh;
}