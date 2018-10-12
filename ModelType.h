#pragma once
#include <GLM\glm.hpp>
#include <vector>
#include "BoundingBox.h"
#include "Joint.h"


class RawModel{
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::uvec3> indices;

	std::vector<glm::vec4> weight;
	std::vector<glm::uvec4> jointId;


	BoundingBox box;
	std::string colorTex;
	std::string normTex;	

	int mFlags=0;
	std::string errorString;
	void resizeAll(size_t t) {
		vertices.resize(t);
		uvs.resize(t);
		normals.resize(t);

		weight.resize(t);
		jointId.resize(t);
	}
	void free() {
		vertices.clear();
		uvs.clear();
		normals.clear();
		indices.clear();
	}
	RawModel() {};
	~RawModel() {};

};

