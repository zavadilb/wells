#include "triangle_mesh.h"
void TriangleMesh::create() {
	Geometry::create(); // Generate vertices on the CPU (it will be stored in <vertices>)

	// send vertices to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, getVertices()->size() * sizeof(VertexData), &(*getVertices())[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);  // attribute array 0 = POSITION
	glEnableVertexAttribArray(1);  // attribute array 1 = NORMAL
	glEnableVertexAttribArray(2);  // attribute array 1 = COLOR
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, color));
}
void TriangleMesh::draw() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, getVertices()->size());
}
