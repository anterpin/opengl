#pragma once
#include "Compute.h"
#include "hk0Shader.h"

class hk0Compute : public Compute{
public:
	static hk0Compute* getInstance(Texture* h0, Texture* _h0,int N) {
		static hk0Compute* instance = nullptr;
		if (instance == nullptr) {
			instance = new hk0Compute(h0,_h0,N);
		}
	}

	hk0Compute(Texture* h0,Texture* _h0,int N) {
		setShader(hk0Shader::getInstance());
		init(h0,N);
		init(_h0,N);
	}
	~hk0Compute() {};

	void compute(Texture* texture,int L,float A,float w,const glm::vec2& W) {
		shader->start();

		((hk0Shader*)shader)->loadVariable(texture[0].getWidth(),L,A,w,W);
		texture[0].bindAndActive(0);
		texture[1].bindAndActive(1);
		texture[2].bindAndActive(2);
		texture[3].bindAndActive(3);

		glBindImageTexture(0, maps[0]->getId(), 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glBindImageTexture(1, maps[1]->getId(), 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute(texture[0].getWidth() / 16, texture[0].getHeight() / 16, 1);

		glFinish();


		shader->stop();
	}

};