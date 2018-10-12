#pragma once


#include "WaterNode.h"


class WaterQuadTree :public Node
{
	PatchVBO buffer;
	WaterConfig* config;
public:
	std::vector<glm::vec2> generateVertexData() {

		std::vector<glm::vec2> vertices(16);
		unsigned int index = 0;

		vertices[index++] = vec2(0, 0);
		vertices[index++] = vec2(0.3333f, 0);
		vertices[index++] = vec2(0.6666f, 0);
		vertices[index++] = vec2(1, 0);

		vertices[index++] = vec2(0, 0.3333f);
		vertices[index++] = vec2(0.3333f, 0.3333f);
		vertices[index++] = vec2(0.6666f, 0.3333f);
		vertices[index++] = vec2(1, 0.3333f);


		vertices[index++] = vec2(0, 0.6666f);
		vertices[index++] = vec2(0.3333f, 0.6666f);
		vertices[index++] = vec2(0.6666f, 0.6666f);
		vertices[index++] = vec2(1, 0.6666f);

		vertices[index++] = vec2(0, 1);
		vertices[index++] = vec2(0.3333f, 1);
		vertices[index++] = vec2(0.6666f, 1);
		vertices[index++] = vec2(1, 1);

		return vertices;
	}
	WaterQuadTree(WaterConfig* config) {

		this->config = config;

		buffer.allocate(generateVertexData());


		for (int i = 0; i < ROOT_NODES; i++) {
			for (int j = 0; j < ROOT_NODES; j++) {
				addChild(new WaterNode(&buffer, config, vec2(i / (float)(ROOT_NODES), j / (float)(ROOT_NODES)), 0, vec2(i, j)));
			}
		}
		getWorldTrasform().setScale(config->getScaleXZ(), config->getScaleY(), config->getScaleXZ());
		getWorldTrasform().setPosition(config->getScaleXZ() / 2.0f, 0, config->getScaleXZ() / 2.0f);
	}
	~WaterQuadTree() {}
	virtual void render() override {
		buffer.bind();

		config->getTotalDisplacement()->bindAndActive(0);
		//config->getNormalMap()->bindAndActive(1);
		config->getLightMap()->bindAndActive(1);
		config->getReflection()->bindAndActive(2);
		config->getRefraction()->bindAndActive(3);

		for (Node* child : getChildren()) {

			((WaterNode*)child)->render();
		}

		buffer.unbind();
	};
	void updateQuadTree() {

		for (Node* child : getChildren()) {
			((WaterNode*)child)->updateQuadTree();
		}
	}
};

