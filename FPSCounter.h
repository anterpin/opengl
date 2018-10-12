#pragma once
#include "FontMesh.h"
#include <ctime>
#include "GuiRenderer.h"
#include "Input.h"

bool countFps(unsigned int& f) {
	static unsigned int fps = 0;
	fps++;
	static clock_t time = clock();
	clock_t time2 = clock();
	if (time2 - time>1000) {
		f = fps;
		fps = 0;
		time = time2;
		return true;
	}
	return false;
}

namespace FPSCounter {
	bool enableDisable() {
		static bool k = true;
		if (Input::isKeyDownRepeat(SDLK_k)) {
			k = !k;
		}
		return k;
	}
	void count() {
		static FontMesh* label = new FontMesh(Font::getDefaultFont(), L"FPS : 0", glm::vec2(0.86, 0.06), glm::vec2(1,1), 0.27f,glm::vec3(1,1,1));
		if (!enableDisable())
			return;

		unsigned int fps;
		if (countFps(fps)) {
			std::wstringstream ss;
			ss << L"FPS : " << fps;
			label->changeText(ss.str());
			//label->changeText(L"Ciao EDU");
			//label->changeText(L"As vezes no silencio da noite , eu fico imaginando nos dois  Eu fico ali sonhando acordado juntando o antes o agora e o depois");
			label->setEffect(glm::vec3(0,1,1),glm::vec2(0.001f, 0.001f), 0.6f, 0.1f);
		}
		GuiRenderer::render(label);

	}

}