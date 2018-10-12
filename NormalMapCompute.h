#pragma once
#include "Texture.h"
#include "NormalMapShader.h"
#include "Compute.h"

struct NormalMapCompute :public Compute{
private:
	float strength;
public:
	inline static NormalMapCompute* getInstance(Texture* map,int N) {
		static NormalMapCompute* instance = nullptr;
		if (instance == nullptr) {
			instance = new NormalMapCompute(map,N);
		}
		return instance;
	}
	inline float getStrength() { return strength; }
	inline void setStrength(float s) { strength = s; }
public:
	NormalMapCompute(Texture* map,int N) {
		setShader(NormalMapShader::getInstance());
		init(map,N,true);
	}
	~NormalMapCompute() {};
	void compute(Texture* texture) {
		shader->start();
		
		((NormalMapShader*) shader)->loadVariable(texture->getWidth(), strength);
		texture->bindAndActive(0);
		glBindImageTexture(0, maps[0]->getId(), 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute(texture->getWidth() / 16, texture->getHeight() / 16, 1);
		glFinish();

		maps[0]->bilinearFilter();

		shader->stop();
	}

};