#pragma once

#include "VBO.h"

class CubeWire : public VBO {

public:
	static CubeWire* getMesh() {
		static CubeWire* cube = nullptr;
		if (!cube)
			cube = new CubeWire();
		return cube;
	}
	CubeWire() {
		genBox();
	}
	void genBox() {

		//		   1____________ 5
		//		   /|          /|
		//		0 /___________/4|
		//		  |	|		 |	|
		//		  |2 _ _ _ _ | _|6
		//		  |	/		 | /
		//		 3|__________|/ 7
		//
		std::vector<glm::vec3> vertices{
			glm::vec3(-1, 1,-1),//0
			glm::vec3(-1, 1, 1),//1
			glm::vec3(-1,-1, 1),//2
			glm::vec3(-1,-1,-1),//3
			glm::vec3( 1, 1,-1),//4
			glm::vec3( 1, 1, 1),//5
			glm::vec3( 1,-1, 1),//6
			glm::vec3( 1,-1, -1)//7
		};
		std::vector<unsigned int> indices{
			0,3,3,2,2,1,1,0,
			4,5,5,6,6,7,7,4,
			0,4,4,7,7,3,3,0,
			0,1,1,5,5,4,4,0,
			2,3,3,7,7,6,6,2,
			1,2,2,6,6,5,5,1
		};

		genVao();
		bindVao();
		storeIndices(indices);
		storeDataInAttribList(0, vertices);
		unbindVao();

		VBO::size = indices.size();

		indices.clear();
		vertices.clear();
	}
	void bindMesh() {
		glLineWidth(3);
		VBO::bindMesh(1);
	}
	void unbindMesh() {
		glLineWidth(1);
		VBO::unbindMesh(1);
	}
	void draw() {
		glDrawElements(GL_LINES, size, GL_UNSIGNED_INT, nullptr);
	}
};

