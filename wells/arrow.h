#ifndef ARROW_H
#define ARROW_H
#include "composite_object.h"
#include "cylinder.h"
#include "cone.h"
#include "vec3.h"

/*
	An arrow (cylinder + cone), which is 1 unit high. By default it points in the positive y direction, and has a uniform green color.
*/
class Arrow : public CompositeObject {
	vec3 coneColor;
	vec3 cylinderColor;
public:

	Arrow(Material* _material, vec3 _coneColor = vec3(0,1,0), vec3 _cylinderColor = vec3(0,1,0)) 
		: CompositeObject(_material), coneColor(_coneColor), cylinderColor(_cylinderColor) {
		for (GameObject* c : children) delete c;
		children.clear();

		const float cylinderLength = 0.75f;
		const float coneLength = 0.25f;

		Cone* cone = new Cone(M_PI / 2.0f, coneLength, coneColor);
		Cylinder* cylinder = new Cylinder(0.1f, cylinderLength, cylinderColor);

		LeafObject* coneObject = new LeafObject(cone, material);
		LeafObject* cylinderObject = new LeafObject(cylinder, material);

		coneObject->rotate(vec3(M_PI, 0, 0)); // \/ ---> /\ 
		coneObject->translate(vec3(0, cylinderLength + coneLength, 0)); // move cone just above cylinder

		add(coneObject);
		add(cylinderObject);
	}
};

#endif // !ARROW_H
