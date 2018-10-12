#pragma once
#include "ShaderProgram.h"

class InversionShader : public ShaderProgram {
private:
	Unii pingpong;
	Unii N;

public:
	InversionShader() :ShaderProgram() {
		init();
	}
	~InversionShader() {};

	inline static InversionShader* getInstance() {
		static InversionShader* instance = nullptr;
		if (!instance) {
			instance = new InversionShader();
		}
		return instance;
	}
	void bindShaders() {
		addComputeShader("Inversion.glsl");
	}
	void getAllUniformLocations() {
		add(N, "N");
		add(pingpong, "pingpong");
	}
	void loadVariable(int N, int pingpong) {
		this->pingpong.useLocation(pingpong);
		this->N.useLocation(N);
	}
	void bindAttributes() {}

};