#pragma once
#include<GLM/glm.hpp>
#include "Model.h"
#include "Trasform.h"

class GameObject
{
protected:
	Trasform trasform;
	Model* model;

public:
	GameObject(Model* model):model(model) {}
	inline Model* getModel() {
		return model;
	};
	inline Trasform& getTrasform() {
		return trasform;
	}
	inline void setTraform(const Trasform& trasform) {
		this->trasform = trasform;
	}
	virtual void update() {
		
	}
	virtual bool isAnimated(){
		return false;
	}

	~GameObject() {};
};

