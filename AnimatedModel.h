#pragma once
#include "Animation.h"
#include "Clock.h"
#include "Model.h"

class AnimatedModel : public Model {
private:
	float animationTime = 0;
	Joint rootJoints = Joint(-1,"");

	Animation* animation;

public:
	AnimatedModel(std::vector<Material*>* materials, std::vector<Mesh*>* meshes,Animation* a,const Joint& root) :Model(materials,meshes){
		animation = a;
		rootJoints = root;
	}
	inline Animation* getAnimation() {
		return animation;
	}
	inline Joint* getArmature() {
		return &rootJoints;
	}
	void bind(unsigned int i)override {
		meshes[i]->bindMesh(5);
	}
	void unbind(unsigned int i)override {
		meshes[i]->unbindMesh(5);
	}
	bool isAnimated()override {
		return true;
	}

	~AnimatedModel() {
		rootJoints.remove();
		delete animation;
	}
};