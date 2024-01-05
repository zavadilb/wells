#ifndef PINK_NOISE_TERRAIN_H
#define PINK_NOISE_TERRAIN_H

#include "param_surface.h"
#define _USE_MATH_DEFINES		// M_PI (on older platforms)
#include <math.h>

class PinkNoiseTerrain : public ParamSurface {
	float W, H; // width, height,
	std::vector<std::vector<float>> a;
	std::vector<std::vector<float>> phi;
	const size_t n = 6;
	const vec3 baseColor = vec3(139, 69, 19); // green
	const vec3 topColor = vec3(50, 205, 50); // brown
	const float baseHeight = -3;
	const float topHeight = 1;

public:
	inline PinkNoiseTerrain(float w = 3, float h = 3) : W(w), H(h) {
		const float a0 = getRandom();
		for (size_t i = 0; i < n; i++) {
			std::vector<float> aRow;
			std::vector<float> phiRow;
			for (size_t j = 0; j < n; j++) {
				float phiVal = 2 * M_PI * (getRandom());
				phiRow.push_back(phiVal);
				float aVal = a0;
				if (i + j > 0) {
					aVal /= sqrtf(i * i + j * j);
				}
				aRow.push_back(aVal);
			}

			a.push_back(aRow);
			phi.push_back(phiRow);
		}

		create(15 * w, 15 * h);
	}
	inline VertexData genVertexData(float u, float v) {
		VertexData vd;
		const float x = u * W;
		const float z = v * H;
		float h = 0; // h(x,z) parametric surface
		vd.normal = vec3(0, 1, 0);
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < n; j++) {
				h += a[i][j] * cosf(i * x + j * z + phi[i][j]);
				vd.normal.x += a[i][j] * i * sinf(i * x + j * z + phi[i][j]); // -1 * (d/dx) h(x,z)
				vd.normal.z += a[i][j] * j * sinf(i * x + j * z + phi[i][j]); // -1 * (d/dz) h(d,z)
			}
		}

		vd.position = vec3(x, h, z);
		float weight = std::max<float>(h, baseHeight);
		weight = std::min<float>(h, topHeight);
		weight -= baseHeight;
		weight /= (topHeight - baseHeight);
		vd.color = weight * baseColor + (1 - weight) * topColor;
		vd.color = normalize(vd.color);
		vd.normal = normalize(vd.normal);
		return vd;
	}
};

#endif // !PINK_NOISE_TERRAIN_H
