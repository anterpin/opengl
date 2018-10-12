#pragma once
#include <GL\glew.h>
#include "Display.h"

class FBO {

protected:
	int WIDTH;
	int HEIGHT;

	unsigned int fbId = 0;
	unsigned int dbId = 0;

public:
	inline unsigned getWidth() { return WIDTH; }
	inline unsigned getHeight() { return HEIGHT; }
	inline unsigned int getBufferId() { return fbId; }
	virtual ~FBO() {}
protected:
	FBO() {
		Display::getWindow()->getSize(WIDTH, HEIGHT);
	}
	FBO(int width, int height) :WIDTH(width), HEIGHT(height){}

	//normal
	virtual void unBindFrameBuffer() {
		int width, height;
		Display::getWindow()->getSize(width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
	}
	virtual void createFrameBuffer(int num=1) {
		glGenFramebuffers(1, &fbId);
		glBindFramebuffer(GL_FRAMEBUFFER, fbId);
		drawMultiColorsAttachment(num);
	}
	virtual void createDepthBuffer() {
		glGenRenderbuffers(1,&dbId);
		
		glBindRenderbuffer(GL_RENDERBUFFER, dbId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WIDTH,HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, dbId);
	}
	void drawMultiColorsAttachment(const size_t& num) {
		unsigned int* colors = new unsigned int[num];
		for (int i = 0; i < num; i++) {
			colors[i] = GL_COLOR_ATTACHMENT0 + i;
		}

		glDrawBuffers((GLsizei)num,colors);

		delete[] colors;
	}
	virtual void check() {
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr<<"ERROR creating frame Buffer"<<std::endl;
			assert(false);
		}
			
	}
	virtual void remove() {
		if (fbId) {
			glDeleteFramebuffers(1,&fbId);
		}
		if (dbId) {
			glDeleteRenderbuffers(1,&dbId);
		}	
	}
	virtual void bindFrameBuffer() {
	
		glBindFramebuffer(GL_FRAMEBUFFER, fbId);
		glViewport(0, 0, WIDTH, HEIGHT);
	}
	//multisampled


};
