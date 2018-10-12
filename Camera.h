#pragma once
#include <GLM\glm.hpp>
#include "Math.h"
#include <GLM/gtx/transform.hpp>
#include <SDL/SDL.h>
#include "Display.h"
#include "Clock.h"




class Camera {
private:
	vec3 position;
	vec3 direction;
	
	//x pitch
	//y yaw
	//z roll
	bool changed = true;
	bool moved = true;
	const float sensibility = 0.1f;
	const float movementFactor = 100;
public:
	glm::mat4 viewMat;
	glm::mat4 projectMat;
	glm::mat4 projectViewMat;
	glm::vec4 frustumPlanes[6];
	
public:
	void createPlanes() {
		math::ExtractPlanes(frustumPlanes, projectViewMat, true);
	}
	void createProjectMat(float FOV=RenderUtils::FOV, 
							float NEAR_PLANE= RenderUtils::NEAR_PLANE, 
							float FAR_PLANE = RenderUtils::FAR_PLANE) {
		float h, w;
		Display::getWindow()->getSize(w, h);

		float aspectRatio = w / h;
		float y_scale = 1.0f / tan(radians(FOV / 2.0f));
		float x_scale = y_scale / aspectRatio;
		float frustum_length = FAR_PLANE - NEAR_PLANE;

		projectMat = mat4();
		projectMat[0][0] = x_scale;
		projectMat[1][1] = y_scale;
		projectMat[2][2] = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
		projectMat[2][3] = -1;
		projectMat[3][2] = -((2 * NEAR_PLANE*FAR_PLANE) / frustum_length);
		projectMat[3][3] = 0;
	}
	void createViewMat() {
		mat4 view(1.0f);
		vec3 dir = math::toRad(direction);
		view = rotate(view, dir.x, vec3(1, 0, 0));
		view = rotate(view, dir.y, vec3(0, 1, 0));
		view = rotate(view, dir.z, vec3(0, 0, 1));
		vec3 negPosition = vec3(-position.x, -position.y, -position.z);

		view = translate(view, negPosition);

		viewMat = view;
	}
	void createViewProjectionMat() {
		projectViewMat = projectMat * viewMat ;
		createPlanes();
	}
	Camera() {
		createProjectMat();

		updateMatrices();
	}
	void move() {
		changed = false;
		moved = false;
		Clock::getInstance()->updateClock();
		int DX = Input::getDX();
		int DY = Input::getDY();
		
		direction.x -= ((float)(DY))*sensibility;
		direction.y -= ((float)(DX))*sensibility;

		bool W_pressed = Input::isKeyDown(SDLK_w);
		bool S_pressed = Input::isKeyDown(SDLK_s);
		bool A_pressed = Input::isKeyDown(SDLK_a);
		bool D_pressed = Input::isKeyDown(SDLK_d);

		float motForward = 0;
		float motSide = 0;

		if (W_pressed || S_pressed || A_pressed || D_pressed)
			moved = changed = true;
		else if (DX || DY)
			changed = true;
		
		if (W_pressed) {
			motForward += 1;
		}
		if (S_pressed) {
			motForward -= 1;
		}
		if (A_pressed) {
			motSide += 1;
		}
		if (D_pressed) {
			motSide -= 1;
		}
		float a = radians(direction.y);
		
		
		motForward *= Clock::getInstance()->getSpaceInTime();
		motSide *= Clock::getInstance()->getSpaceInTime();
		position.z -= motForward*movementFactor*cos(a);
		position.x += motForward*movementFactor*sin(a);
		position.y -= motForward*movementFactor*sin(radians(direction.x));

		position.x -= motSide*movementFactor * cos(a);
		position.z -= motSide*movementFactor * sin(a);

		updateMatrices();
		//std::cout << Camera::getInstance()->getPosition().x << " ";
		//std::cout <<this->getPosition().y << "\n";
		//std::cout << Camera::getInstance()->getPosition().z << "\n";
	}
	void updateMatrices() {
		createViewMat();
		createViewProjectionMat();
	}
	__forceinline void invertPitch() { direction.x = -direction.x; changed = true; }
	__forceinline void invertRoll() { direction.z = -direction.z; }
	__forceinline bool isChanged() {return changed;}
	__forceinline bool isMoved() { return moved; }
	__forceinline void setPosition(vec3 position) {
		this->position = position;
	};
	__forceinline void setPosition(float x, float y, float z) {
		setPosition(vec3(x, y, z));
	};
	__forceinline void setDirection(vec3 direction) {
		this->direction = direction;
	};
	__forceinline void setDirection(float x, float y, float z) {
		setDirection(vec3(x, y, z));
	};
	__forceinline vec3 getDirection() { return direction; }
	__forceinline vec3 getPosition() { return position; }
	__forceinline void dicreasePosY(float v) { position.y-=v; }
	__forceinline static Camera* getInstance() { 
		/*static Camera* instance=nullptr;
		if (!instance)
			instance = new Camera();*/
		static Camera* instance = new Camera();
		return instance; 
	}
	void printMat(const glm::mat4& m) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				std::cout << m[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
};
