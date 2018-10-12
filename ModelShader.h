#pragma once
#include"ShaderProgram.h"
#include "AmbientLight.h"
#include "Camera.h"
#include "ImporterUtils.h"

using namespace glm;

class ModelShader : public ShaderProgram
{
	Unim4 trasformMat;

	Univ4 plane;

	Univ3 ambient;

	Unim4 jointTrasforms[50];
	Unib hasAnimation;

public:
	inline static ModelShader* getInstance() {
		static ModelShader* instance=nullptr;
		if (!instance) {
			instance = new ModelShader();
		}
		return instance;
	}
	ModelShader() :ShaderProgram() {
		init();
	}
	~ModelShader() {

	}
	void bindShaders() override{
		addVertexShader("model_VS.glsl");
		addGeometryShader("model_GS.glsl");
		addFragmentShader("model_FS.glsl");
	}
	void getAllUniformLocations() {
		add(trasformMat,"trasformMatrix");
		add(ambient, "ambient");	
		add(plane, "plane");

		add(hasAnimation, "hasAnimation");

		for (int i = 0; i < 50; i++) {
			std::stringstream ss;
			ss << "jointTrasforms[" << i << "]";
			add(jointTrasforms[i], ss.str());
		}
	}
	void bindAttributes() override {
		bindAttrib(0, "position");
		bindAttrib(1, "uv");
		bindAttrib(2, "normal");
		bindAttrib(3, "jointIds");
		bindAttrib(4, "weights");
	}
	inline void updateArrayJTrasf(std::vector<glm::mat4>* v) {
		hasAnimation.useLocation(true);

		for (int i = 0; i < v->size();i++) {
			jointTrasforms[i].useLocation(v->at(i));
			//std::cout << i << std::endl;
			//ImporterUtils::printMat(v->at(i));
		}
	}
	inline void updateUniforms() {
		//load("ambient", AmbientLight::getAmbient()->getColor());
		ambient.useLocation(AmbientLight::getAmbient()->getColor());
		hasAnimation.useLocation(false);
	
		plane.useLocation(RenderUtils::clipPlane());
	}
	inline void loadTraformMatrix(const mat4& mat) {
		//load("trasformMatrix", mat);
		trasformMat.useLocation(mat);
	};

};

