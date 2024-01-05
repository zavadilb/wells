#ifndef CFG_H
#define CFG_H
#include "vec3.h"
#include "vec4.h"
// Screen resolution
const unsigned int WINDOW_WIDTH_PX = 1920, WINDOW_HEIGHT_PX = 1080;

// Debugging
extern bool WIREFRAME_ONLY;
extern bool BACK_FACE_CULLING;
extern bool DISPLAY_NORMALS;
const float DISPLAY_NORMALS_MAGNITUDE = 0.15f;
const vec4 DISPLAY_NORMALS_COLOR = vec4(1,1,0,1);

/* Global physics constants */
static struct PHYSICS {
public:
	static vec3 g; // gravitational acceleration
	static const float G; // gravitational constant
	static const float rho; // Linear translational (air) drag coefficient
	static const float kappa; // Linear rotational (air) drag coefficient
	static const float c; // rate of time change constant
};

#endif // !CFG_H