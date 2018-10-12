#pragma once
#include <GLM\glm.hpp>
#include "Clock.h"

class AmbientLight {
private:
	glm::vec3 color;
	
public:
	static AmbientLight* getAmbient() {
		static AmbientLight* amb = new AmbientLight();
		return amb;
	}
	void osciling() {
		static float v = 1;
		static int rising = -1;
		if (v < 0) {
			v = 0;
			rising = -1;
		}
		if (v > 1) {
			v = 1;
			rising = 1;
		}
		v-=rising*Clock::getInstance()->getSpaceInTime();
		setColor(v);
	}
	AmbientLight(float x , float y , float z ) {
		color = glm::vec3(x, y, z);
	}
	AmbientLight(float x):AmbientLight(x,x,x) {}
	AmbientLight() : AmbientLight(1,1,1){}
	~AmbientLight() {}
	const glm::vec3& getColor() {
		return color;
	}
	void setColor(const glm::vec3& _color) {
		color = _color;
	}
	void setColor(float v) {
		setColor(glm::vec3(v, v, v));
	}

};