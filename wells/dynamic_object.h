#ifndef DYNAMIC_OBJECT_H
#define DYNAMIC_OBJECT_H

#include "leaf_object.h"
#include "cfg.h"
//TODO for now this is only for non rotating objects with homogeneous weight distribution (i. e. spheres :/)
class DynamicObject :
    public LeafObject
{
    vec3 currentNetForce; // current net force applied to the object
    float m; // weight of the object
    vec3 p = vec3(0, 0, 0); // momentum
public:
    DynamicObject(Geometry* g, Material* m, float mass);
    void animate(float tstart, float tend);
    void setCurrentNetForce(vec3 newForce);
    float getMass();
};

#endif // !DYNAMIC_OBJECT_H
