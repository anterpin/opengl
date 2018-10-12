#pragma once
#include "ShaderProgram.h"

class FFTTwiddleFactorShader : public ShaderProgram {
private:
	Unii N;

public:
	FFTTwiddleFactorShader() :ShaderProgram() {
		init();
	}
	~FFTTwiddleFactorShader() {};

	inline static FFTTwiddleFactorShader* getInstance() {
		static FFTTwiddleFactorShader* instance = nullptr;
		if (!instance) {
			instance = new FFTTwiddleFactorShader();
		}
		return instance;
	}
	void bindShaders() {
		addComputeShader("TwiddleFactors.glsl");
	}
	void getAllUniformLocations() {
		add(N, "N");
	}
	void loadVariable(unsigned int N) {
		this->N.useLocation(N);
	}
	void bindAttributes() {}

};
