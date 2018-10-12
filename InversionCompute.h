#pragma once
#include "Compute.h"
#include "InversionShader.h"

class InversionCompute : public Compute {
	int N;
public:
	InversionCompute(Texture* dx,Texture* dy,Texture* dz ,int N) :N(N) {
		init(dx,N);
		init(dy, N);
		init(dz, N);
		setShader(InversionShader::getInstance());
	}
	void compute(int displacentCompontent, int pingpong) {

		shader->start();

		((InversionShader*)shader)->loadVariable(N, pingpong);

		glBindImageTexture(0, maps[displacentCompontent]->getId(), 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
		glDispatchCompute(N / 16, N / 16, 1);
		glFinish();


	}
};
