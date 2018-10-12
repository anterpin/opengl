#pragma once
#include "ShaderProgram.h"

class UnifyDistShader : public ShaderProgram {
private:
	Unif strength;

public:
	UnifyDistShader() :ShaderProgram() {
		init();
	}
	~UnifyDistShader() {};

	inline static UnifyDistShader* getInstance() {
		static UnifyDistShader* instance = nullptr;
		if (!instance) {
			instance = new UnifyDistShader();
		}
		return instance;
	}
	void bindShaders() {
		addComputeShader("unifyDistortion.glsl");
	}
	void getAllUniformLocations() {
		add(strength, "strength");
	}
	void loadVariable(float strength) {
		this->strength.useLocation(strength);
	}
	void bindAttributes() {}

};