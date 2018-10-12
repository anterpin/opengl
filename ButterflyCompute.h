#pragma once
#include "Compute.h"
#include "ButterflyShader.h"

class ButterflyCompute : public Compute {
	int N;
public:
	ButterflyCompute(Texture* t,int N) :N(N) {
		setShader(ButterflyShader::getInstance());
		init(t,N);
	}
	void compute(Texture* twiddle,Texture* displaceComponent,int &pingpong) {
		shader->start();
		int log_2_N = (int)(std::log(N) / std::log(2));

		glBindImageTexture(0, twiddle->getId(), 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
		glBindImageTexture(1, displaceComponent->getId(), 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
		glBindImageTexture(2, maps[0]->getId(), 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
		for (int i = 0; i<log_2_N; i++)
		{
			((ButterflyShader*)shader)->loadVariable(pingpong, 0, i);
			glDispatchCompute(N / 16, N / 16, 1);
			glFinish();
			pingpong++;
			pingpong %= 2;
		}

		//1D FFT vertical 
		for (int j = 0; j<log_2_N; j++)
		{
			((ButterflyShader*)shader)->loadVariable(pingpong, 1, j);
			glDispatchCompute(N / 16, N / 16, 1);
			glFinish();
			pingpong++;
			pingpong %= 2;
		}

		
	}
};
