#ifndef SURFACE_SHADER_H
#define SURFACE_SHADER_H
#include "shader.h"
class SurfaceShader : public Shader {
public:
	void load(const char* const vertexSourceFileName, const char* const fragmentSourceFileName, const char* const geometrySourceFileName = nullptr);
	virtual void bind(RenderState state);
};
#endif /*SURFACE_SHADER_H*/

