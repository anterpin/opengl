#pragma once
#include "Node.h"
#include "WaterQuadTree.h"
#include "AABB.h"
#include "AABBShader.h"
#include "Cube.h"

class Water : public Node
{
private:
	WaterConfig config;
	
public:
	Water() {};
	~Water() {};

	inline WaterConfig* getConfig() {
		return &config;
	}
	inline void setConfig(const WaterConfig& config) {
		this->config = config;
	}
	inline float getShift() {
		return config.getShift();
	}
	inline void setShift(float shift) {
		config.setShift(shift);
	}
	void Init(std::string path) {
		config.loadFile(path);

		addChild(new WaterQuadTree(&config));

	}
	void updateQuadTree() {
		Camera* camera = Camera::getInstance();
		if (std::abs(camera->getPosition().y - getShift()) < 300) {
			static unsigned long prevTime = clock();
			unsigned long currentTime = clock();
			if (currentTime - prevTime > 1000.0f / 24.0f)//update every 1/24 second
			{
				prevTime = currentTime;
				config.updateSurface();
			}
			
		}
			
		if (camera->isMoved()) {
			((WaterQuadTree*)getChildren()[0])->updateQuadTree();
		}
	}
	inline void setReflRefract(Texture* refl, Texture* refr) {
		config.setReflection(refl);
		config.setRefraction(refr);
	}
};
