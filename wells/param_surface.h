#ifndef PARAM_SURFACE_H
#define PARAM_SURFACE_H
#include "geometry.h"
#include "util.h"
#include <vector>
#include "opengl_platform.h"
#include "cfg.h"
class ParamSurface : public Geometry {
	unsigned int nVtxPerStrip, nStrips;
	int N = 0;
	int M = 0;
	bool limitN = true;
	bool limitM = true;

	// the allocated memory will be freed by Geometry upon destruction.
	std::vector<VertexData>* generateAllVertices();
public:
	ParamSurface();

	virtual VertexData genVertexData(float u, float v) = 0;

	void create(int N = 40, int M = 40, bool limitN = true, bool limitM = true);
	void draw();
};
#endif // !PARAM_SURFACE_H

