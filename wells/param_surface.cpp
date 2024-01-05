#include "param_surface.h"
ParamSurface::ParamSurface() { nVtxPerStrip = nStrips = 0; }

std::vector<VertexData>* ParamSurface::generateAllVertices() {
	if (limitN) {
		N = N > 30 ? N : 30; // For very small objects, we still want a few triangles
		N = N < 200 ? N : 200; // Limit the max amount of triangles
	}
	if (limitM) {
		M = M > 30 ? M : 30; // For very small objects, we still want a few triangles
		M = M < 200 ? M : 200; // Limit the max amount of triangles
	}

	//printf("Creating parametric surface on %d*%d grid, with %d triangles.", N, M, N * (M + 1) * 2);
	//if (!limitN) printf(" (unlimited N)");
	//if (!limitM) printf(" (unlimited M)");
	//printf("\n");

	std::vector<VertexData>* vtxData = new std::vector <VertexData>();
	nVtxPerStrip = (M + 1) * 2;
	nStrips = N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j <= M; j++) {
			vtxData->push_back(genVertexData((float)j / M, (float)i / N));
			vtxData->push_back(genVertexData((float)j / M, (float)(i + 1) / N));
		}
	}
	return vtxData; // Geometry manages the memory from now on
}

void ParamSurface::create(int N, int M, bool limitN, bool limitM) {
	this->N = N;
	this->M = M;
	this->limitN = limitN;
	this->limitM = limitM;

	Geometry::create(); // create the vertex data on the CPU (it gets stored in <vertices>)

	//send generated vertex data to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, nVtxPerStrip * nStrips * sizeof(VertexData), &(*getVertices())[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);  // attribute array 0 = POSITION
	glEnableVertexAttribArray(1);  // attribute array 1 = NORMAL
	glEnableVertexAttribArray(2);  // attribute array 2 = COLOR
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, color));
}
void ParamSurface::draw() {
	glBindVertexArray(vao);
	for (unsigned int i = 0; i < nStrips; i++)
		glDrawArrays(GL_TRIANGLE_STRIP, i * nVtxPerStrip, nVtxPerStrip);
}