#pragma once
#include <GLM\glm.hpp>
#include "Camera.h"
#include "MasterRenderer.h"
#include "UBO.h"

class CameraBlock : UBO {
private:
	Camera* instance = nullptr;
	struct Block {
		glm::vec4 cameraPos;
		glm::mat4 viewMat;
		glm::mat4 projectMat;
		glm::mat4 projectViewMat;
		float zFar;
		float zNear;
	}block;
public:
	static CameraBlock* getInstance() {
		static CameraBlock* instance = nullptr;
		if (!instance) {
			instance = new CameraBlock(Camera::getInstance());
		}
		return instance;
	}
	CameraBlock(Camera* camera):instance(camera){
		block.cameraPos = glm::vec4(camera->getPosition(),1.0);
		block.viewMat = camera->viewMat;
		block.projectMat = camera->projectMat;
		block.projectViewMat = camera->projectViewMat;
		block.zFar = RenderUtils::FAR_PLANE;
		block.zNear = RenderUtils::NEAR_PLANE;

		genUbo();
		bind();
		allocate(this->toPtr(), this->getSize());

		unbind();
	}
	inline void updateWithoutMoving() {
		if (!instance->isChanged())
			return;
		instance->updateMatrices();
		block.cameraPos = glm::vec4(instance->getPosition(), 1.0);
		block.viewMat = instance->viewMat;
		block.projectViewMat = instance->projectViewMat;

		bind();

		updateBlockSubData(this->toPtr(), block.viewMat);
		updateBlockSubData(this->toPtr(), block.cameraPos);
		updateBlockSubData(this->toPtr(), block.projectViewMat);
		unbind();
		//updateBlockMap(this->toPtr(), this->getSize());
	}
	inline Camera* getCamera() {
		return instance;
	}
	void update() {
		instance->move();
		updateWithoutMoving();
	}
	inline signed char* toPtr() {
		return (signed char*)&block;
	}
	inline size_t getSize() {
		return sizeof(Block);
	}
	inline unsigned int getId() {
		return UBO::getId();
	}
	inline void bind() {
		UBO::bind();
	}
	inline void unbind() {
		UBO::unbind();
	}

};
