#pragma once

#include "Node.h"
#include "PatchVBO.h"
#include "WaterConfig.h"
#include "Camera.h"
#include "oceanShader.h"


class WaterNode : public Node
{
private:
	bool isLeaf;
	WaterConfig* config;
	unsigned int lod;
	vec2 location;
	vec3 worldPos;
	vec2 index;
	float gap;
	PatchVBO* buffer;
public:
	inline bool getLeaf() { return isLeaf; }
	inline WaterConfig* getConfig() { return config; }
	inline void setConfig(WaterConfig* config) { this->config = config; }
	inline vec2 getLocation() { return location; }
	inline void setLocation(const vec2& location) { this->location = location; }
	inline vec3 getWorldLocation() { return worldPos; }
	inline void setWorldLocation(const vec3& worldLPos) { this->worldPos = worldPos; }
	inline float getGap() { return gap; }
	inline vec2 getIndex() { return index; }
	inline unsigned int getLod() { return lod; }
private:
	void addChildNodes(unsigned int lod) {
		if (isLeaf) {
			isLeaf = false;
		}
		if (getChildren().size() == 0) {
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					addChild(new WaterNode(buffer, config, location + vec2(i*gap / 2.0f, j*gap / 2.0f), lod, vec2(i, j)));
				}
			}
		}
	}
	void removeChildNodes() {
		if (!isLeaf) {
			isLeaf = true;
		}
		if (getChildren().size() != 0) {
			for (Node* node : getChildren())
				delete (WaterNode*)(node);
			getChildren().clear();
		}
	}

public:
	WaterNode(PatchVBO* buffer, WaterConfig* config, const vec2& location, unsigned int lod, const vec2& index) :buffer(buffer), config(config), location(location), lod(lod), index(index) {
		isLeaf = true;
		gap = 1.0f / (ROOT_NODES*(float)(glm::pow(2, lod)));


		getLocalTrasform().setScale(gap, 0, gap);
		getLocalTrasform().setPosition(location.x, 0, location.y);

		getWorldTrasform().setScale(config->getScaleXZ(), config->getScaleY(), config->getScaleXZ());
		getWorldTrasform().setPosition(-config->getScaleXZ() / 2.0f, 0, -config->getScaleXZ() / 2.0f);

		computeWorldPos();
		updateQuadTree();
	}
	~WaterNode() {};
	int intersectCulling() {
		const float scaleXZ = config->getScaleXZ();

		glm::vec3 _min = (getLocalTrasform().getPosition() - 0.5f)*scaleXZ;
		_min.y = config->getShift();

		glm::vec3 _max = (getLocalTrasform().getPosition() + gap - 0.5f)*scaleXZ;
		_max.y = config->getShift() + config->getScaleY();

		AABB box(_max, _min);

		return box.intersectCulling(glm::mat4(1.0f));
		/*AABBShader* s = AABBShader::getInstance();
		s->start();
		Cube* cube = Cube::getMesh();
		cube->bindMesh();
		s->loadVariable(box.getTrasformMat(), false);
		cube->draw();
		cube->unbindMesh();*/

		//return false;
	}
	bool frustumCulling() {
		const float scaleXZ = config->getScaleXZ();

		glm::vec3 _min = (getLocalTrasform().getPosition() - 0.5f)*scaleXZ;
		_min.y = config->getShift();

		glm::vec3 _max = (getLocalTrasform().getPosition() + gap - 0.5f)*scaleXZ;
		_max.y = config->getShift() + config->getScaleY();

		AABB box(_max, _min);
		
		return box.frustumCulling(glm::mat4(1.0f));
		/*AABBShader* s = AABBShader::getInstance();
		s->start();
		Cube* cube = Cube::getMesh();
		cube->bindMesh();
		s->loadVariable(box.getTrasformMat(), false);
		cube->draw();
		cube->unbindMesh();*/
		
		//return false;
	}
	void render() {
		if (isLeaf) {
			getWorldTrasform().setPosY(0);
			//OceanShader::getInstance()->start();
			OceanShader::getInstance()->loadPerObjectVar(config, getLocalTrasform(),
				getWorldTrasform());

			buffer->draw();
		}
		if (!frustumCulling()) {
			return;
		}
		/*if (test) {
			if (!frustumCulling()) {
				return;
			}
		}
		test = false;*/
		/*if (test) {
			//int i = intersectCulling();
			int i = intersectCulling();
			if (!i) {
				return;
			}
			test = (i == -1);
		}*/
	
		
		for (Node* child : getChildren()) {
			((WaterNode*)child)->render();
		}
	}
	void updateQuadTree() {
		if (Camera::getInstance()->getPosition().y > config->getScaleY()) {
			worldPos.y = config->getScaleY();
		}
		else {
			worldPos.y = Camera::getInstance()->getPosition().y;
		}

		updateChildNodes();

		for (Node* node : getChildren()) {
			((WaterNode*)node)->updateQuadTree();
		}
	}
	void updateChildNodes() {

		float distance = glm::length(Camera::getInstance()->getPosition() - worldPos);


		if (distance < config->getLodRange()[lod]) {
			addChildNodes(lod + 1);
		}
		else {
			removeChildNodes();
		}
	}
	void computeWorldPos() {
		vec2 loc = ((location + (gap / 2.0f))*config->getScaleXZ()) - (config->getScaleXZ() / 2.0f);

		this->worldPos = vec3(loc.x, 0, loc.y);
	}

};