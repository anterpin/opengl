#pragma once
#include "Texture2D.h"
#include "FBO.h"
#include "GameObject.h"
#include "Terrain.h"
#include <vector>
#include "Render.h"
#include "GuiRenderer.h"
#include "CameraBlock.h"

class FrameBuffer : public FBO {
private:
	std::vector<Texture2D> colors;
	Texture2D depth;
	bool useBuffer;
public:
	//hey

	inline Texture* getColorTexture(int i=0) { return &colors[i]; }
	inline Texture* getDepthTexture() {return &depth;}
	~FrameBuffer()override {
		FBO::remove();
   }
	FrameBuffer(int width , int height) :FBO(width, height) {
		useBuffer = false;
		init(false,1);
	}
	FrameBuffer() :FBO(1080,720) {
		useBuffer = false;
		init(false, 1);
	}
	FrameBuffer(bool depthBuffer,int numColors,int width=1080, int height=720) :FBO(width, height) {
		useBuffer = depthBuffer;
		init(depthBuffer,numColors);
	}
	void init(bool depthBuffer,int numColors) {
		colors.resize(numColors);
		createFrameBuffer();
		for (Texture2D& color : colors) {
			color.createTextureAttachment(FBO::getWidth(), FBO::getHeight());
		}
		if (depthBuffer) {
			createDepthBuffer();
			depth.createEmptyTexture(FBO::getWidth(), FBO::getHeight());
		}
		else {
			depth.createDepthTextureAttachment(FBO::getWidth(), FBO::getHeight());
		}
		check();
		unBindFrameBuffer();
	}
	void BufferToTexture() {
		depth.bind();
		glReadBuffer(GL_BACK); // Ensure we are reading from the back buffer.
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,0, 0, FBO::getWidth(), FBO::getHeight(), 0);
	}
	void bind() {
		glBindTexture(GL_TEXTURE_2D, 0);
		bindFrameBuffer();
	}
	void unbind() {
		if (useBuffer) {
			BufferToTexture();
		}
		unBindFrameBuffer();
		
	}
	void renderRefraction(std::vector<GameObject*>* objects,Terrain* terrain,float height=0) {
		RenderUtils::enableClipPlane();
		bind();
		RenderUtils::clipPlane(-1, -height);
		Render::RenderScene(objects, terrain);
		unbind();
		RenderUtils::disableClipPlane();
	}
	void renderColorTexture(Box* content,int i=0) {
		GuiRenderer::render(content, &colors[i]);
	}
	void renderDepthTexture(Box* content) {
		GuiRenderer::render(content, &depth,true);
	}
	void renderReflection(std::vector<GameObject*>* objects, Terrain* terrain, float height=0) {
		RenderUtils::enableClipPlane();
		CameraBlock* block = CameraBlock::getInstance();
		Camera* camera = block->getCamera();

		float distance = 2 * (camera->getPosition().y - height);
		camera->dicreasePosY(distance);
		camera->invertPitch();
		camera->invertRoll();
		block->updateWithoutMoving();

		bind();
		RenderUtils::clipPlane(1, height);
		Render::RenderScene(objects, terrain);
		unbind();

		camera->dicreasePosY(-distance);
		camera->invertPitch();
		camera->invertRoll();
		block->updateWithoutMoving();

		RenderUtils::disableClipPlane();
	}


};