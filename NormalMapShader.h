#pragma once
#include "ShaderProgram.h"



class NormalMapShader : public ShaderProgram {
private:
	Unii heightmap;
	Unii N;
	Unif strength;

public:
	NormalMapShader() :ShaderProgram(){
		init();
	}
	~NormalMapShader() {};

	inline static NormalMapShader* getInstance() {
		static NormalMapShader* instance = nullptr;
		if (instance == nullptr) {
			instance = new NormalMapShader();
		}
		return instance;
	}
	void bindShaders() {
		addComputeShader("normalMap.glsl");
	}
	void getAllUniformLocations() {
		add(heightmap, "heightmap");
		add(N, "N");
		add(strength, "strength");
	}
	void loadVariable(unsigned int N,float strength) {
		this->strength.useLocation(strength);
		this->heightmap.useLocation(0);
		this->N.useLocation(N);
	}
	void bindAttributes() {

	}

};