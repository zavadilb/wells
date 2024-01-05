#ifndef COMPOSITE_OBJECT_H
#define COMPOSITE_OBJECT_H
#include "game_object.h"
#include <stdexcept>
#include <vector>

class CompositeObject : public GameObject {
protected:
	std::vector<GameObject*> children;

public:
	CompositeObject(Material* _material) : GameObject(_material) {}

	virtual void animate(float tstart, float tend) {}
	vec3 findFurthestPoint(vec3 dir, mat4 transform = mat4()) {
		if (children.size() == 0) throw new std::runtime_error("Error calculating GJK support function: composite object has no children!");
		const mat4 nextTransform = getModelMx() * transform;
		
		vec3 maxSupport = children[0]->findFurthestPoint(dir, nextTransform);
		float maxSupportDot = dot(maxSupport, dir);

		for (size_t i = 1; i < children.size(); i++)
		{
			// add this composite object's model matrix to the child's wPositions
			const vec3 currentSupport = children[i]->findFurthestPoint(dir, nextTransform);
			const float currentSupportDot = dot(currentSupport, dir);

			if (currentSupportDot > maxSupportDot) {
				maxSupport = currentSupport;
				maxSupportDot = currentSupportDot;
			}
		}

		return maxSupport;
	}

	void add(GameObject* child) {
		if (child == NULL) return;

		child->setShader(shader);
		children.push_back(child);
	}
	void remove(GameObject* child) {
		if (child == NULL) return;

		children.erase(std::remove(children.begin(), children.end(), child), children.end());
	}
	void draw(RenderState state) {
		if (shader == NULL) {
			throw new std::invalid_argument("Object cannot be drawn: no shader set. (Did you forget to call setShader() before draw()?)");
		}

		state.M = getModelMx() * state.M;
		state.Minv = state.Minv * getModelInvMx();

		for (GameObject* c : children) c->draw(state);
	}
	void setShader(Shader* newShader) {
		shader = newShader;
		for (GameObject* c : children) c->setShader(shader);
	}

	~CompositeObject() {
		if (children.size() > 0) for (GameObject* c : children) delete c;
		if (material) delete material;
	}
};

#endif // !COMPOSITE_OBJECT_H

