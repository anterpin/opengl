#pragma once
#include "ShaderProgram.h"
#include "Clock.h"


class hktShader : public ShaderProgram {
private:
	Unii N;
	Unii L;
	Unif t;
	//Unii hk0;
	//Unii hkminus0;

public:
	hktShader() :ShaderProgram() {
		init();
	}
	~hktShader() {};

	inline static hktShader* getInstance() {
		static hktShader* instance = nullptr;
		if (!instance) {
			instance = new hktShader();
		}
		return instance;
	}
	void bindShaders() {
		addComputeShader("hkt.glsl");
	}
	void getAllUniformLocations() {
		add(N, "N");
		add(L, "L");
		add(t, "t");
	}
	void loadVariable(unsigned int N,int _L) {
		this->N.useLocation(N);
		L.useLocation(_L);
		
		t.useLocation(Clock::getInstance()->getSeconds()*4);

	}
	void bindAttributes() {}

};
