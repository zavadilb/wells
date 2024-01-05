#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "util.h"
#include "shader.h"
class GameObject {
private:
	// M and Minv should only be manipulated through modelChanged by subclasses, and should be accessed by getters.
	mat4 M; // model matrix
	mat4 Minv; // inverse model matrix

protected:
	Shader* shader;
	Material* material;
	vec3 position; // x, y, z coordinates to where the object should be translated when drawn
	vec3 orientation; // (phiX, phiY, phiZ) (rotation angle around X, Y and Z axis)
	vec3 size;
	
	// call if position, orientation or size changed and the effect of the change has to be visible the next time the object is drawn
	virtual void modelChanged() {
		M = ScaleMatrix(size) * RotationMatrix(orientation) * TranslateMatrix(position);
		Minv = TranslateMatrix(-1.0f * position)* RotationMatrix(-1.0f * orientation)* ScaleMatrix(divide(1.0f, size));
	}
public:
	GameObject(Material* _material, vec3 _position = vec3(0,0,0), vec3 _orientation = vec3(0,0,0), vec3 _scale = vec3(1,1,1)) :
		shader(NULL), position(_position), orientation(_orientation), material(_material), size(_scale) {}

	mat4 getModelMx() { return M; }
	mat4 getModelInvMx() { return Minv; }
	vec3 getPosition() { return position; }
	vec3 getOrientation() { return orientation; }
	virtual void scale(vec3 scaleVector) { 
		size = hadamard(size, scaleVector); 
		modelChanged();
	}
	virtual void setScale(vec3 scaleVector) { 
		size = scaleVector;
		modelChanged();
	}
	virtual void translate(vec3 translationVector) { 
		position = applyTransform(position, TranslateMatrix(translationVector)); 
		modelChanged();
	}
	virtual void setPosition(vec3 newPosition) {
		position = newPosition; 
		modelChanged();
	}
	virtual void rotate(vec3 xyz) { 
		orientation = orientation + xyz; 
		modelChanged();
	} // rotate around x,y,z axis by (phiX, phiY, phiZ)
	virtual void setOrientation(vec3 xyz) { 
		orientation = xyz; 
		modelChanged();
	} // rotate around x,y,z axis by (phiX, phiY, phiZ)
	virtual void setShader(Shader* newShader) { shader = newShader; }

	virtual void draw(RenderState state) = 0;
	virtual void animate(float tstart, float tend) = 0;	
	virtual vec3 findFurthestPoint(vec3 dir, mat4 transform = mat4()) = 0; // transform is applied to each wPosition before support point calculation. Transform defaults to unit matrix

	virtual ~GameObject() { if (shader) delete shader; }
};
#endif // !GAME_OBJECT_H

