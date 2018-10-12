#pragma once
#include <GL\glew.h>
#include <GLM\glm.hpp>
#include "AmbientLight.h"




namespace RenderUtils{
	
	const static float RED = 0.0f;
	const static float GREEN = 0.2f;
	const static float BLUE = 0.3f;
	const static float ALPHA = 1.0f;

	const static float FOV = 70;
	const static float NEAR_PLANE = 1.0f;
	const static float FAR_PLANE = 10000;

	__forceinline void enableBackCulling() {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	__forceinline const glm::vec4& clipPlane(float pointingNormalY=0,float d=0) {
		static glm::vec4 plane(0, 1, 0, 1);
		if (pointingNormalY) {
			plane.y = pointingNormalY; plane.w = d;
		}
		return plane;
	}
	__forceinline void enableClipPlane(unsigned int plane = 0) {glEnable(GL_CLIP_DISTANCE0+plane);}
	__forceinline void disableClipPlane(unsigned int plane = 0) {glDisable(GL_CLIP_DISTANCE0+plane);}
	__forceinline void enableMultiSample() { glEnable(GL_MULTISAMPLE); }
	__forceinline void disableMultiSample() { glDisable(GL_MULTISAMPLE); }
	__forceinline void enableDepthTest() {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}
	__forceinline void disableDepthTest() {
		glDisable(GL_DEPTH_TEST);
	}
	__forceinline void enableFrontCulling() {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}
	__forceinline void disableCulling() {
		glDisable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	}
	__forceinline void enableWireFrame() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	__forceinline void disableWireFrame() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	__forceinline bool changeWireFrame(bool test=false) {
		static bool enable = false;
		if (test)
			return enable;

		if (enable)
			disableWireFrame();
		else
			enableWireFrame();

		enable = !enable;
		return enable;
	}
	__forceinline void enableAlphaBlend() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	__forceinline void disableAlphaBlend() {
		glDisable(GL_BLEND);
	}
	__forceinline void clearColor() {
		glm::vec3 v(RED, GREEN, BLUE);
		v = v* AmbientLight::getAmbient()->getColor();
		glClearColor(v.x, v.y, v.z, ALPHA);
	}
	__forceinline void clearDepthBuffer() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}




