#include "geometry.h"
Geometry::Geometry() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}
void Geometry::create() {
	vertices = generateAllVertices();
}
Geometry::~Geometry() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	delete vertices;
}