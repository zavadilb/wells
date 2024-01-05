#ifndef SHADOW_DEPTH_SHADER_H
#define SHADOW_DEPTH_SHADER_H

#include "surface_shader.h"
class ShadowDepthShader :
    public SurfaceShader
{
public:
    void bind(RenderState state);
};

#endif // !SHADOW_DEPTH_SHADER_H