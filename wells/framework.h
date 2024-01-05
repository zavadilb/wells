//=============================================================================================
// Collection of programs from lecture slides.
// Framework for assignments. Valid from 2020.
//
// Do not change it if you want to submit a homework.
// In the homework, file operations other than printf are prohibited.
//=============================================================================================
#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#define _USE_MATH_DEFINES		// M_PI (on older platforms)

// basic CPP includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>

// project includes
#include "gpu_program.h"
#include "mat4.h"
#include "texture.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "util.h"
#include "shader.h"
#include "surface_shader.h"
#include "triangle.h"
#include "geometry.h"
#include "triangle_mesh.h"
#include "cuboid_mesh.h"
#include "param_surface.h"
#include "sphere.h"
#include "pink_noise_terrain.h"
#include "game_object.h"
#include "leaf_object.h"
#include "camera.h"
#include "body.h"
#include "scene.h"
#include "cfg.h" // settings, constants
#include "cylinder.h"
#include "cone.h"
#include "normal_display_shader.h"
#include "arrow.h"
#include "composite_object.h"
#include "well.h"
#include "dynamic_object.h"
#include "shadow_depth_shader.h"

// opengl (platform-specific)
#include "opengl_platform.h"

#endif // !FRAMEWORK_H