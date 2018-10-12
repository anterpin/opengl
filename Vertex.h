#pragma once
#include <map>
#include<GLM\glm.hpp>


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal = glm::vec3(0,0,0);
	unsigned int index;
	int normalIndex = -1;
	int uvIndex = -1;

	int initlialPosIndex = -1;
	Vertex* duplicate = nullptr;
	bool isSet() {
		return uvIndex != -1 && normalIndex != -1;
	}
	bool hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther)const {
		return textureIndexOther == uvIndex && normalIndexOther == normalIndex;
	}
	bool hasSameTexture(int textureIndexOther)const {
		return textureIndexOther == uvIndex;
	}
	void addNormal(const glm::vec3& v) {
		normal += v;
		//normal = glm::normalize(normal);
		if (duplicate) {
			duplicate->setNormalRecursevly(normal);
		}
	}
	void setNormalRecursevly(const glm::vec3& v) {
		normal = v;
		if (duplicate) {
			duplicate->setNormalRecursevly(v);
		}
	}
};

