#include "surface_shader.h"

void SurfaceShader::load(const char* const vertexSourceFileName, const char* const fragmentSourceFileName, const char* const geometrySourceFileName) {
	// read fragment and vertex shaders from file
	std::ifstream vertexSourceStream(vertexSourceFileName);
	std::ifstream fragmentSourceStream(fragmentSourceFileName);

	std::stringstream buffer;
	buffer << vertexSourceStream.rdbuf();
	std::string vertexSource = buffer.str();

	std::stringstream buffer2;
	buffer2 << fragmentSourceStream.rdbuf();
	std::string fragmentSource = buffer2.str();

	if (geometrySourceFileName != nullptr) {
		std::ifstream geometrySourceStream(geometrySourceFileName);
		std::stringstream buffer3;
		buffer3 << geometrySourceStream.rdbuf();
		std::string geometrySource = buffer3.str();

		create(vertexSource.c_str(), fragmentSource.c_str(), "outColor", geometrySource.c_str());
	}
	else {
		create(vertexSource.c_str(), fragmentSource.c_str(), "outColor");
	}
}

void SurfaceShader::bind(RenderState state) {
	Use();
	setUniformMaterial(state.material, "material");

	setUniform(state.MVP, "MVP");
	setUniform(state.M, "M");
	setUniform(state.Minv, "Minv");
	setUniform(state.wEye, "wEye");
	setUniform(state.lightVP, "lightVP");

	setUniformMaterial(state.material, "material");

	setUniformLight(state.light, "light");
}