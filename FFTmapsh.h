#pragma once
#include "Texture2D.h"
#include "hk0Compute.h"
#include "hktCompute.h"
#include "TwiddleIndicesCompute.h"
#include "ButterflyCompute.h"
#include "InversionCompute.h"
#include "unifyDistShader.h"
#include "NormalMapCompute.h"


struct FFTOcean {
private:
	float A;
	int L;
	float w;
	glm::vec2 W;

	Texture2D tilde_hk0;
	Texture2D tilde_hkminus0;
	
	Texture2D tilde_dyComponent;
	Texture2D tilde_dxComponent;
	Texture2D tilde_dzComponent;

	Texture2D Dy;
	Texture2D Dx;
	Texture2D Dz;

	Texture2D twiddleIndices;
	Texture2D pingpong;

	hktCompute* hkt_compute;
	ButterflyCompute* b_compute;
	InversionCompute* i_compute;
	NormalMapCompute* normalMapCompute;

	Texture2D uniDist;
	Texture2D normalMap;

	int N;
	int log_2_N;
	
	bool choppy = true;
public:
	FFTOcean() :A(15.0f), L(2000), w(23.0f), W(glm::vec2(1,0.5)) {};
	FFTOcean(float A,int L,float w,const glm::vec2& W):A(A),L(L),w(w),W(W) {};
	~FFTOcean() { 
		delete hkt_compute;
		delete b_compute;
		delete i_compute;
	};
	void init(Texture* noise) {

		N = noise[0].getWidth();
		log_2_N = (int)(std::log(N) / std::log(2));

		hkt_compute = new hktCompute(&tilde_dyComponent, &tilde_dxComponent, &tilde_dzComponent,N);
		TwiddleCompute twCompute(&twiddleIndices,N);

		hk0Compute c(&tilde_hk0, &tilde_hkminus0, N);
		c.compute(noise,L,A,w,W);
		twCompute.compute();

		
		b_compute = new ButterflyCompute(&pingpong, N);

		i_compute = new InversionCompute(&Dx, &Dy, &Dz, N);

		uniDist.generate();
		uniDist.bind();
		uniDist.storage2D(N, N);

		
		normalMap.generate();
		normalMap.bind();
		
		normalMap.storage2D(N, N);
		normalMap.trilinearFilter();
	}
	void update() {
		hkt_compute->compute(&tilde_hk0, &tilde_hkminus0,L);

		int ping = 0;

		b_compute->compute(&twiddleIndices, &tilde_dyComponent, ping);
		i_compute->compute(1, ping);
		if(choppy){

			ping = 0;
			b_compute->compute(&twiddleIndices, &tilde_dxComponent, ping);
			i_compute->compute(0, ping);

			ping = 0;
			b_compute->compute(&twiddleIndices, &tilde_dzComponent, ping);
			i_compute->compute(2, ping);

		}

		UnifyDistShader::getInstance()->start();
		UnifyDistShader::getInstance()->loadVariable(3.0f);
		glBindImageTexture(0, uniDist.getId(), 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
		glBindImageTexture(1, Dx.getId(), 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
		glBindImageTexture(2, Dy.getId(), 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
		glBindImageTexture(3, Dz.getId(), 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
		glBindImageTexture(4, normalMap.getId(), 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
		glDispatchCompute(N / 16, N / 16, 1);
		glFinish();

	}

	inline Texture* get_hk0(){
		return &tilde_hk0;
	}
	inline Texture* get_hkminus0() {
		return &tilde_hkminus0;
	}
	inline Texture* getDYComponent() {
		return &tilde_dyComponent;
	}
	inline Texture* getDXComponent() {
		return &tilde_dxComponent;
	}
	inline Texture* getDZComponent() {
		return &tilde_dzComponent;
	}
	inline Texture* getTwiddleIndices() {
		return &twiddleIndices;
	}
	inline Texture* getPingPong() {
		return &pingpong;
	}
	inline Texture* getDYDisplacement() {
		return &Dy;
	}
	inline Texture* getDXDisplacement() {
		return &Dx;
	}
	inline Texture* getDZDisplacement() {
		return &Dz;
	}
	inline Texture* getTotalDisplacement() {
		return &uniDist;
	}
	inline Texture* getNormalMap(){
		return &normalMap;
	}

	
};