#pragma once
#include "ShaderProgram.h"


class hk0Shader : public ShaderProgram {
private:
	Unii noise_r0;
	Unii noise_i0;
	Unii noise_r1;
	Unii noise_i1;

	Unii N;
	Unii L;
	Unif A;
	Univ2 w;
	Unif windspeed;

public:
	hk0Shader() :ShaderProgram() {
		init();
	}
	~hk0Shader() {};

	inline static hk0Shader* getInstance() {
		static hk0Shader* instance = nullptr;
		if (instance == nullptr) {
			instance = new hk0Shader();
		}
		return instance;
	}
	void bindShaders() {
		addComputeShader("h0.glsl");
	}
	void getAllUniformLocations() {
		add(noise_r0, "noise_r0");
		add(N, "N");
		add(noise_r1, "noise_r1");

		add(noise_i0, "noise_i0");
		add(L, "L");
		add(noise_i1, "noise_i1");

		add(A, "A");
		add(w, "w");
		add(windspeed, "windspeed");
	}
	void loadVariable(unsigned int N,int L,float A,float windspeed,const glm::vec2& w) {

		this->N.useLocation(N);
		this->L.useLocation(L);
		this->A.useLocation(A);
		this->w.useLocation(normalize(w));
		this->windspeed.useLocation(windspeed);

		noise_r0.useLocation(0);
		noise_i0.useLocation(1);
		noise_r1.useLocation(2);
		noise_i1.useLocation(3);
	}
	void bindAttributes() {}

};
