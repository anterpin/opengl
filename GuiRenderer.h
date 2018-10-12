#pragma once
#include "GuiShader.h"
#include "FontMesh.h"
#include "MasterRenderer.h"
#include "Box.h"
#include "ImageShader.h"

namespace GuiRenderer {
	static GuiShader* guiShader=nullptr;
	static ImageShader* imageShader = nullptr;
	void init() {
		guiShader = GuiShader::getInstance();
		imageShader =  ImageShader::getInstance();
	}
	inline bool prepare() {
		RenderUtils::disableCulling();
		RenderUtils::disableDepthTest();
		RenderUtils::enableAlphaBlend();

		bool wasEnableWire = RenderUtils::changeWireFrame(true);

		if (wasEnableWire)
			RenderUtils::changeWireFrame();

		return wasEnableWire;
	}
	inline void terminate(bool wire) {
		RenderUtils::enableBackCulling();
		RenderUtils::enableDepthTest();
		RenderUtils::disableAlphaBlend();

		if (wire)
			RenderUtils::changeWireFrame();
	}
	void render(FontMesh* m) {
		bool wire = prepare();
		guiShader->start();
		m->bindMesh();
		for (int i = 0; i < m->getFont()->fontAtlas.size(); i++) {
			m->getFont()->fontAtlas[i].bindAndActive(i);
		}
		guiShader->loadVar(m->getColor(), m->getTrasform());
		guiShader->loadEffects(m->getOffset(), m->getOutLineColor(), m->getBorderWidth(),m->getBorderEdge());
		m->draw();
	
		m->unbindMesh();

		//guiShader->stop();

		terminate(wire);
		
	}
	void render(Box* b, Texture* image, bool linearDepth = false) {
		bool wire = prepare();
		imageShader->start();
		b->bind();
		image->bindAndActive(0);


		imageShader->loadVariable(b->getTrasformationMatrix(), b->getTrasparency(),b->getInvertedTex(),image, linearDepth);
		b->draw();
		b->unbind();

		terminate(wire);
	}
	void render(Box* b) {
		bool wire = prepare();
		imageShader->start();
		b->bind();
		b->getTexture()->bindAndActive(0);

		imageShader->loadVariable(b->getTrasformationMatrix(),b->getTrasparency(),b->getInvertedTex(), b->getTexture());
		b->draw();
		b->unbind();

		terminate(wire);
	}
	void free() {
		if (!guiShader) {
			delete guiShader;
			guiShader = nullptr;
		}
		if (!imageShader) {
			delete imageShader;
			imageShader = nullptr;
		}
	}


}
