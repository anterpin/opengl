#pragma once
#include <iostream>
#include <GLM\glm.hpp>
#include "VBO.h"
#include "BoundingSphere.h"
#include "Camera.h"
#include "AABB.h"

class BoundingBox {
private:
	glm::vec3 max;
	glm::vec3 min;
	CubeWire* cube;
public:
	void initForRender() {
		cube = CubeWire::getMesh();
	}
	void draw(){
		cube->draw();
	}
	glm::vec3 getCorner(unsigned int i) {
		switch (i) {
		case 0 :
			return glm::vec3(min.x, max.y, min.z);
		case 1:
			return glm::vec3(min.x, max.y, max.z);
		case 2:
			return glm::vec3(min.x, min.y, max.z);
		case 3:
			return min;
		case 4:
			return glm::vec3(max.x, max.y, min.z);
		case 5:
			return max;
		case 6:
			return glm::vec3(max.x, min.y, max.z);
		case 7:
			return glm::vec3(max.x, min.y, min.z);

		}
		return glm::vec3(0, 0, 0);
	}
	inline glm::vec3 getLUF() {
		return glm::vec3(min.x, max.y, min.z);
	}
	inline glm::vec3 getLUB() {
		return glm::vec3(min.x, max.y, max.z);
	}
	inline glm::vec3 getLBB() {
		return glm::vec3(min.x, min.y, max.z);
	}
	inline glm::vec3 getRUF() {
		return glm::vec3(max.x, max.y, min.z);
	}
	inline glm::vec3 getRBB() {
		return glm::vec3(max.x, min.y, max.z);
	}
	inline glm::vec3 getRBF() {
		return glm::vec3(max.x, min.y, min.z);
	}
	
	mat4 getTrasformMat(Trasform& tras) {
		glm::vec3 center = (max + min) / 2.0f;
		return tras.trasformationMatrix()* math::Trasl(center)* math::Scal(max - center);
	}
	glm::vec3 getCenter() {
		return  (max + min) / 2.0f;
	}
	AABB getAABB(const quat& rot) {
		AABB axisAlignBB;
		
		for (int i = 0; i < 8; i++) {
			axisAlignBB.limitsCheck(rot *getCorner(i));
		}

		return axisAlignBB;

	}
	void limitsCheck(const glm::vec3& v) {
		//std::cout << v.x << " " << v.y << " " << v.z << std::endl;
		if (v.x < min.x) {
			min.x = v.x;
		}
		else if (v.x > max.x) {
			max.x = v.x;
		}

		if (v.y < min.y) {
			min.y = v.y;
		}
		else if (v.y > max.y) {
			max.y = v.y;
		}

		if (v.z < min.z) {
			min.z = v.z;
		}
		else if (v.z > max.z) {
			max.z = v.z;
		}

	}
	void print() {
		std::cout << min.x << " "
			<< max.x << " "
			<< min.y << " "
			<< max.y << " "
			<< min.z << " "
			<< max.z << std::endl;
	}
	void bindMesh() {
		cube->bindMesh();
	}
	void unbindMesh() {
		cube->unbindMesh();
	}
	int getArea() {
		glm::vec3 sub = max - min;
		return sub.x*sub.y*sub.z;
	}
	BoundingSphere boundingSphere() {
		BoundingSphere sphere;
		sphere.center = (max+ min)/2.0f;
		sphere.radius = length(max - sphere.center);
		return sphere;
	}
};