#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H
#include "geometry.h"
#include "util.h"
#include "opengl_platform.h"
#include <vector>
#include "cfg.h"

class TriangleMesh : public Geometry {
protected:
	// the allocated memory will be freed by Geometry upon destruction.
	std::vector<VertexData>* generateAllVertices() = 0;
public:
	void create();
	void draw();
};
#endif // !TRIANGLE_MESH_H
