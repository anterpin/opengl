#pragma once
#include "ShaderProgram.h"

class ButterflyShader : public ShaderProgram {
private:
	Unii stage;
	Unii pingpong;
	Unii direction;

public:
	ButterflyShader() :ShaderProgram() {
		init();
	}
	~ButterflyShader() {};

	inline static ButterflyShader* getInstance() {
		static ButterflyShader* instance = nullptr;
		if (!instance) {
			instance = new ButterflyShader();
		}
		return instance;
	}
	void bindShaders() {
		addComputeShader("Butterfly.glsl");
	}
	void getAllUniformLocations() {
		add(stage, "stage");
		add(pingpong, "pingpong");
		add(direction, "direction");
	}
	void loadVariable(int pingpong,int dir,int stage) {
		this->pingpong.useLocation(pingpong);
		this->direction.useLocation(dir);
		this->stage.useLocation(stage);
	}
	void bindAttributes() {}

};