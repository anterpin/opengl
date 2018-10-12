#pragma once
#include <GLM\glm.hpp>


class BoundingSphere {
#define PI 3.141592653f
public:
	glm::vec3 center;
	float radius;
	float getArea() {
		return (radius*radius*radius)*PI* 4.0f/3.0f;
	}
};
