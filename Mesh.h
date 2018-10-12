#pragma once
#include <GL\glew.h>
#include "ModelType.h"
#include <iostream>
#include "VBO.h"

class Mesh : public VBO{
private:
	
public:
	BoundingBox boundingBox;
	void draw();
	inline unsigned int getVaoId() { return vaoId; }

	Mesh();
	void loadToVao(RawModel& mesh,bool anim = false);
	void bindMesh(unsigned int i=3) {
		VBO::bindMesh(i);
	}
	void unbindMesh(unsigned int i = 3){
		VBO::unbindMesh(i);
	}
};

