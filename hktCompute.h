#pragma once
#include "Compute.h"
#include "hktShader.h"

class hktCompute : public Compute {
public:
	static hktCompute* getInstance(Texture* dy, Texture* dx, Texture* dz,int N) {
		static hktCompute* instance = nullptr;
		if (instance == nullptr) {
			instance = new hktCompute(dy,dx,dz,N);
		}
		return instance;
	}

	hktCompute(Texture* dy, Texture* dx, Texture* dz,int N) {
		setShader(hktShader::getInstance());
		init(dy,N);
		init(dx,N);
		init(dz,N);
	}
	~hktCompute() {};
	void compute(Texture* h0, Texture* _h0,int L) {
		shader->start();

		((hktShader*)shader)->loadVariable(h0->getWidth(),L);

		glBindImageTexture(0, maps[0]->getId(), 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glBindImageTexture(1, maps[1]->getId(), 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glBindImageTexture(2, maps[2]->getId(), 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glBindImageTexture(3, h0->getId(), 0, false, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(4, _h0->getId(), 0, false, 0, GL_READ_ONLY, GL_RGBA32F);
		glDispatchCompute(h0->getWidth() / 16, h0->getHeight()/ 16, 1);

		glFinish();

		shader->stop();
	}

};