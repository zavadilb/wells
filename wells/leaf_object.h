#ifndef LEAF_OBJECT_H
#define LEAF_OBJECT_H
#include "game_object.h"
#include "geometry.h"
#include "surface_shader.h"
#include <stdexcept>
class LeafObject : public GameObject {
protected:
	Geometry* geometry;
	std::vector<vec3>* const wPositions = new std::vector<vec3>(); // vertex positions of the geometry transformed to world space
public:
	LeafObject(Geometry* g, Material* m) : GameObject(m), geometry(g) {
		//create wPositions
		std::vector<VertexData>* const vertices = geometry->getVertices();
		for (size_t i = 0; i < vertices->size(); i++)
		{
			wPositions->push_back(applyTransform((*vertices)[i].position, getModelMx()));
		}
	}
	virtual void animate(float tstart, float tend) {}
	void modelChanged() {
		GameObject::modelChanged();

		//update wVertices
		std::vector<VertexData>* const vertices = geometry->getVertices();
		for (size_t i = 0; i < vertices->size(); i++)
		{
			// no new vertices are added, so no need to reallocate
			(*wPositions)[i] = applyTransform((*vertices)[i].position, getModelMx());
		}
	}
	void draw(RenderState state) {
		if (shader == NULL) {
			throw new std::invalid_argument("Object cannot be drawn: no shader set. (Did you forget to call setShader() before draw()?)");
		}

		state.M = getModelMx() * state.M;
		state.Minv = state.Minv * getModelInvMx();
		state.MVP = state.M * state.V * state.P;
		state.material = material;
		shader->bind(state);
		geometry->draw();
	}
	vec3 findFurthestPoint(vec3 dir, mat4 transform = mat4()) {

		if (wPositions->size() == 0) throw new std::runtime_error("Error calculating GJK support function: leaf object has no positions (vertices)!");

		vec3 maxSupport = applyTransform((*wPositions)[0], transform);
		float maxSupportDot = dot(maxSupport, dir);

		for (size_t i = 1; i < wPositions->size(); i++)
		{
			const vec3 currentSupport = applyTransform((*wPositions)[i], transform);
			const float currentSupportDot = dot(currentSupport, dir);
			if (currentSupportDot > maxSupportDot) {
				maxSupportDot = currentSupportDot;
				maxSupport = currentSupport;
			}
		}

		return maxSupport;
	}

	~LeafObject() {
		if (geometry) delete geometry;
		if (material) delete material;
	}
};
#endif // !LEAF_OBJECT_H

