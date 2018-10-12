#pragma once
#include <GL\glew.h>
#include <GLM\glm.hpp>
#include <string>
#include <vector>
#include <sstream>
#include "Math.h"

struct Joint {

	const int index;
	const std::string name;

	std::vector<Joint*> children;

	glm::mat4 inverseBindMat;
	glm::mat4 animatedTrasform;
	Joint() :index(-1), name("ROOT") {}
	Joint(const int index, const std::string& name):index(index),name(name) {}
	Joint(const Joint& j) :index(j.index), name(j.name) {
		inverseBindMat = j.inverseBindMat;
		children.resize(j.children.size());
		for (int i = 0; i < j.children.size(); i++) {
			children[i] = j.children[i];
		}
	}
	void operator  =(const Joint& j) {
		inverseBindMat = j.inverseBindMat;
		children = j.children;
	}
	Joint(const int index, const glm::vec3& rot, const std::string& name)
		:index(index), name(name) {
		inverseBindMat = glm::mat4(1.0f);
	}
	void setInverseBindMat(const glm::mat4& m) {
		inverseBindMat = m;
	}
	glm::mat4& getInverseBindMatRef() {
		return inverseBindMat;
	}
	~Joint() {}
	void addChild(Joint* j) {
		children.push_back(j);
	}
	bool NameInChildren(const std::string& s) {
		for (Joint* j : children) {
			if (j->name == s)
				return true;
		}
		return false;
	}
	void printIds(unsigned int level = 0) {
		for (Joint* j : children) {
			std::stringstream ss;
			for (int i = 0; i < level; i++) {
				ss << "   ";
			}
			ss << j->index << " "<<j->name;
			std::cout << ss.str() << std::endl;
			j->printIds(level + 1);
		}
	}
	void remove() {
		for (Joint* j : children) {
			if (!j)
				delete j;
		}
	}
};
