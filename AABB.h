#pragma once
#include <GLM\glm.hpp>
#include "Math.h"
#include "Trasform.h"
#include "CubeWire.h"

class AABB {
	glm::vec3 max;
	glm::vec3 min;

public:
	AABB() {}
	AABB(const glm::vec3& max, const glm::vec3 &min){
		this->max = max;
		this->min = min;
	}
	void render(CubeWire* cube) {
		cube->bindMesh();
		cube->draw();
		cube->unbindMesh();
	}
	bool frustumCulling(const glm::mat4& M) {
		glm::vec4 _max = M*glm::vec4(max, 1);
		glm::vec4 _min = M*glm::vec4(min, 1);

		glm::vec4* p = Camera::getInstance()->frustumPlanes;

		return math::insideFrustumAABB(p, glm::vec3(_max.x, _max.y, _max.z), glm::vec3(_min.x, _min.y, _min.z));
	}
	int intersectCulling(const glm::mat4& M) {
		glm::vec4 _max = M*glm::vec4(max, 1);
		glm::vec4 _min = M*glm::vec4(min, 1);

		glm::vec4* p = Camera::getInstance()->frustumPlanes;

		return math::intersectFrustumAABB(p, glm::vec3(_max.x, _max.y, _max.z), glm::vec3(_min.x, _min.y, _min.z));
	}
	mat4 getTrasformMat(Trasform& tras) {
		glm::vec3 center = (max + min) / 2.0f;
		return tras.traslScaleMatrix() * math::Trasl(center)* math::Scal(max - center);
	}
	mat4 getTrasformMat() {
		glm::vec3 center = (max + min) / 2.0f;
		return math::Trasl(center)* math::Scal(max - center);
	}
	glm::vec3 getCenter() {
		return  (max + min) / 2.0f;
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
};
