#ifndef SHADER_H
#define SHADER_H
#include "util.h"
#include "gpu_program.h"
#include <iostream>
#include <sstream>
#include <fstream>

class Shader : public GPUProgram {
public:
	virtual void bind(RenderState state) = 0;

	void setUniformMaterial(const Material* material, const std::string& name);
	void setUniformLight(const Light& light, const std::string& name);
};

#endif // !SHADER_H