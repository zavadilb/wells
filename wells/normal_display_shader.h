#ifndef NORMAL_DISPLAY_SHADER_H
#define NORMAL_DISPLAY_SHADER_H
#include "surface_shader.h"
#include "cfg.h"
class NormalDisplayShader :
    public SurfaceShader
{
public:
    void bind(RenderState state);
};
#endif // !NORMAL_DISPLAY_SHADER_H

