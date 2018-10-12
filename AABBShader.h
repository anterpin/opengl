#pragma once
#include "ShaderProgram.h"

class AABBShader : public ShaderProgram {
private:
	Unim4 trasform;
	Unib intersect;

	Univ4 plane;

public:
	AABBShader() :ShaderProgram() {
		init();
	}
	~AABBShader() {};

	inline static AABBShader* getInstance() {
		static AABBShader* instance = nullptr;
		if (instance == nullptr) {
			instance = new AABBShader();
		}
		return instance;
	}
	void bindShaders() {
		addVertexShader("aabbShader_VS.glsl");
		addFragmentShader("aabbShader_FS.glsl");
	}
	void getAllUniformLocations() {
		add(intersect, "intersect");
		add(trasform, "trasformMat");

		add(plane, "plane");
	}
	void loadVariable(const glm::mat4& tras, bool inter) {
		trasform.useLocation(tras);
		intersect.useLocation(inter);

		plane.useLocation(RenderUtils::clipPlane());
	}
	void bindAttributes() {
		bindAttrib(0, "position");
	}

};