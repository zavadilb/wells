#include "normal_display_shader.h"
void NormalDisplayShader::bind(RenderState state) {
	Use();
	setUniform(state.V, "V");
	setUniform(state.M, "M");
	setUniform(state.M, "Minv");
	setUniform(DISPLAY_NORMALS_MAGNITUDE, "MAGNITUDE");
	setUniform(DISPLAY_NORMALS_COLOR, "color");
	setUniform(state.P, "P");
}
