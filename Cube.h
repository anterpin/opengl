#pragma once

#include "VBO.h"

class Cube: public VBO {

public:
	static Cube* getMesh() {
		static Cube* cube = nullptr;
		if (!cube)
			cube = new Cube();
		return cube;
	}
	Cube() {
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
			glm::vec3(1, 1,-1),//4
			glm::vec3(1, 1, 1),//5
			glm::vec3(1,-1, 1),//6
			glm::vec3(1,-1, -1)//7
		};
		std::vector<unsigned int> indices{
			0,3,2,1,
			4,5,6,7,
			0,4,7,3,
			0,1,5,4,
			2,3,7,6,
			1,2,6,5
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
		VBO::bindMesh(1);
	}
	void unbindMesh() {
		VBO::unbindMesh(1);
	}
	void draw() {
		glDrawElements(GL_QUADS, size, GL_UNSIGNED_INT, nullptr);
	}
};


