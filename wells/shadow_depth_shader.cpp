#include "shadow_depth_shader.h"
void ShadowDepthShader::bind(RenderState state) {
	Use();
	setUniform(state.M, "M");
	setUniform(state.lightVP, "lightVP");
}