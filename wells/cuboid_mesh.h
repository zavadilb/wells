#ifndef CUBOID_MESH_H
#define CUBOID_MESH_H

#include "triangle_mesh.h"
#include "triangle.h"
#include <vector>

class CuboidMesh : public TriangleMesh {
	vec3 position;
	vec3 scale;
	vec3 color;
	vec3 topCenter;
	std::vector<VertexData>* generateAllVertices();
public:
	vec3 compX, compY, compZ; // x, y and z components
	CuboidMesh(vec3 position = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1), vec3 color = vec3(1, 1, 1));
};

#endif // !CUBOID_MESH_H
