#include "cuboid_mesh.h"
std::vector<VertexData>* CuboidMesh::generateAllVertices() {
	std::vector<Triangle> triangles;
	triangles.push_back(Triangle(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f)));
	triangles.push_back(Triangle(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)));
	triangles.push_back(Triangle(vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)));
	triangles.push_back(Triangle(vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f)));
	triangles.push_back(Triangle(vec3(1.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f)));
	triangles.push_back(Triangle(vec3(1.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)));
	triangles.push_back(Triangle(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)));
	triangles.push_back(Triangle(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)));
	triangles.push_back(Triangle(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f)));
	triangles.push_back(Triangle(vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f)));
	triangles.push_back(Triangle(vec3(1.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f)));
	triangles.push_back(Triangle(vec3(1.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)));

	compX = vec3(1.0, 0, 0) * scale.x;
	compY = vec3(0, 1.0, 0) * scale.y;
	compZ = vec3(0, 0, 1.0) * scale.z;

	std::vector<VertexData>* mesh = new std::vector<VertexData>();
	for (size_t i = 0; i < triangles.size(); i++) { // order of triangle definitions important!
		for (int j = 0; j < 3; j++) {
			// scale
			triangles[i].vertices[j].x *= scale.x;
			triangles[i].vertices[j].y *= scale.y;
			triangles[i].vertices[j].z *= scale.z;

			// move to specified position
			triangles[i].vertices[j].x += scale.x * -0.5f + position.x;
			triangles[i].vertices[j].y += scale.y * -0.5f + position.y;
			triangles[i].vertices[j].z += scale.z * -0.5f + position.z;

			VertexData vd;
			vd.position = triangles[i].vertices[j];
			vd.normal = triangles[i].normal;
			vd.color = color;
			mesh->push_back(vd);
		}
	}
	return mesh;
}
CuboidMesh::CuboidMesh(vec3 position, vec3 scale, vec3 color) {
	this->position = position;
	this->scale = scale;
	this->color = color;
	create();
}