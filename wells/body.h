#ifndef BODY_H
#define BODY_H
#include "leaf_object.h"
#include "vec3.h"
#include "cuboid_mesh.h"
#include "cfg.h"

/*
 Represents a cuboid bouncing on a string. The cuboid's center is by default placed at (0,0,0), and the rope fixation point has to be
 given in the constructor (ropeFixedAt). Translation, rotation and scaling only scales the object, it does not affect the
 rope fixation point. The body can only rotate along the xy plane.
*/
class Body : public LeafObject {
	vec3 p = vec3(0, 0, 0); // momentum
	float m; // mass
	vec3 L = vec3(0, 0, 0); // angular momentum
	vec3 s; // rope fixation point
	vec3 l; // point where rope is connected to body
	float I; // moment of inertia

	const float l0 = 0.2f; // length after which the spring pulls on the object
	const float D = 20; // Spring (rope) constant

	CuboidMesh* bodyMesh; // keeping track of the body mesh

	// recalculates rope fixation point
	void calcRopeFixationPoint();
	// recalculates moment of inertia
	void calcMomentOfInertia();
public:

	Body(Material* _material, vec3 ropeFixedAt, float _m);
	void animate(float tstart, float tend);

	void translate(vec3 translationVector);
	void setPosition(vec3 newPosition);
	void rotate(vec3 xyz); // rotate around x,y,z axis by (phiX, phiY, phiZ)
	void setOrientation(vec3 xyz); // rotate around x,y,z axis by (phiX, phiY, phiZ)
	void setVelocity(vec3 v);
	void scale(vec3 scaleVector);
	void setScale(vec3 scaleVector);

	~Body();
};
#endif // !BODY_H


