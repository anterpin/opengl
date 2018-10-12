#pragma once
#include "Texture2DMultisample.h"
#include "FBO.h"
#include "GameObject.h"
#include "Terrain.h"
#include <vector>
#include "Render.h"
#include "FrameBuffer.h"

class FrameBufferMultisample : public FBO {
private:
	std::vector<unsigned int> cbIds;//color buffers

	std::vector<Texture2DMultisample> cTextures;//color buffers
	Texture2DMultisample depth;
public:
	inline Texture2DMultisample* getColorTexture(unsigned int i=0) {
		return &cTextures[i];
	}
	inline Texture2DMultisample* getDepthTexture() {
		return &depth;
	}
	~FrameBufferMultisample()override {
		FBO::remove();
		for (unsigned int& id : cbIds) {
			if (id) {
				glDeleteRenderbuffers(1, &id);
			}
		}
	}
	FrameBufferMultisample(int width = 1080, int height = 720) :FBO(width, height) {
		init(false,1);
	}
	FrameBufferMultisample(int numColors, int width = 1080, int height = 720) :FBO(width, height) {
		init(true,numColors);
	}
	FrameBufferMultisample(bool depthBuffer, int width = 1080, int height = 720) :FBO(width, height) {
		init(depthBuffer,1);
	}
	void init(bool depthBuffer,int size,int MSAA = 4) {
		if (depthBuffer) {
			cbIds.resize(size);
			for (int i = 0; i < cbIds.size(); i++) {
				// create a 4x MSAA renderbuffer object for colorbuffer
					glGenRenderbuffers(1, &cbIds[i]);
					glBindRenderbuffer(GL_RENDERBUFFER, cbIds[i]);
					glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA, GL_RGB8, WIDTH, HEIGHT);
				}
			// create a 4x MSAA renderbuffer object for depthbuffer
			glGenRenderbuffers(1, &dbId);
			glBindRenderbuffer(GL_RENDERBUFFER, dbId);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA, GL_DEPTH_COMPONENT24, WIDTH, HEIGHT);
		}
		else {
			cTextures.resize(size);
			for (int i = 0; i < cTextures.size(); i++) {
				std::cout << i << std::endl;

				cTextures[i].createTextureAttachment(WIDTH,HEIGHT,MSAA);

			}
			depth.createDepthTextureAttachment(WIDTH, HEIGHT, MSAA);
		}		
		

		// create a 4x MSAA framebuffer object
		glGenFramebuffers(1, &fbId);
		glBindFramebuffer(GL_FRAMEBUFFER, fbId);

		if (depthBuffer) {
			drawMultiColorsAttachment(cbIds.size());
			for (int i = 0; i < cbIds.size(); i++) {
				// attach colorbuffer image to FBO
				glFramebufferRenderbuffer(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
					GL_COLOR_ATTACHMENT0 + i, // 2. color attachment point
					GL_RENDERBUFFER,      // 3. rbo target: GL_RENDERBUFFER
					cbIds[i]);          // 4. rbo ID
			}
			// attach depthbuffer image to FBO
			glFramebufferRenderbuffer(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
				GL_DEPTH_ATTACHMENT,  // 2. depth attachment point
				GL_RENDERBUFFER,      // 3. rbo target: GL_RENDERBUFFER
				dbId);          // 4. rbo ID
								// check FBO status
		}	
		else {
			drawMultiColorsAttachment(cTextures.size());
			for (int i = 0; i < cTextures.size(); i++) {
				cTextures[i].setAsColorAttachment(i);
			}
			depth.setAsDepthAttachment();
		}

		check();
		unBindFrameBuffer();
	}
	void resolveToNormalFrameBuffer(FrameBuffer* buf,int attachment=0) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buf->getBufferId());
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->getBufferId());
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
		glBlitFramebuffer(0, 0, FBO::getWidth(), FBO::getHeight(), 0, 0, buf->getWidth(), buf->getHeight(),
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,GL_NEAREST);

		this->unBindFrameBuffer();
	}
	void resolveToScreen() {
		int w, h;
		Display::getWindow()->getSize(w, h);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->getBufferId());
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glDrawBuffer(GL_BACK);
		glBlitFramebuffer(0, 0, FBO::getWidth(), FBO::getHeight(), 0, 0, w,h,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);

		this->unBindFrameBuffer();
	}
	void bind() {
		bindFrameBuffer();
	}
	void unbind() {
		unBindFrameBuffer();
	}



};
