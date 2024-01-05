#include "shader.h"

void Shader::setUniformMaterial(const Material* material, const std::string& name) {
	if (material != NULL) {
		setUniform(material->kd, name + ".kd");
		setUniform(material->ks, name + ".ks");
		setUniform(material->ka, name + ".ka");
		setUniform(material->shininess, name + ".shininess");
	}
	else {
		setUniform(vec3(0, 0, 0), name + ".kd");
		setUniform(vec3(0, 0, 0), name + ".ks");
		setUniform(vec3(1, 1, 1), name + ".ka");
		setUniform(0, name + ".shininess");
	}
}

void Shader::setUniformLight(const Light& light, const std::string& name) {
	setUniform(light.La, name + ".La");
	setUniform(light.Le, name + ".Le");
	setUniform(light.wLightPos, name + ".wLightPos");
}
