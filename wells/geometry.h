#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "opengl_platform.h"
#include "vec3.h"
#include "util.h"
#include <vector>
class Geometry {
private:
	std::vector<VertexData>* vertices = new std::vector<VertexData>(); // vertices on the CPU

protected:
	unsigned int vao, vbo;

	// the allocated memory will be freed by Geometry upon destruction.
	virtual std::vector<VertexData>* generateAllVertices() = 0;
	// create vertex data on the CPU
	void create();

public:
	Geometry();

	std::vector<VertexData>* getVertices() { return vertices; }

	virtual void draw() = 0;

	virtual ~Geometry();
};

#endif // !GEOMETRY_H

