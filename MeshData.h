#pragma once
#include "ModelType.h"


struct MeshData {

	std::vector<RawModel> meshes;

	//Skealeton
	Joint root = Joint(-1, glm::vec3(0, 0, 0), "ROOT");
	bool hasAnimation = false;

};