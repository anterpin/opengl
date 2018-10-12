#pragma once
#include "Render.h"
#include "GuiRenderer.h"
#include "FPSCounter.h"
#include "AmbientLight.h"




class Engine {
private:
	Display* window = nullptr;
public:
	static Engine* getEngine() {
		static Engine* m_engine = nullptr;
		if (!m_engine) {
			m_engine = new Engine();
		}
		return m_engine;
	}
	Engine() {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			std::cerr << "Unable to initialize SDL:" << SDL_GetError() << std::endl;
			assert(false);
		}
	}
	~Engine() {
		Render::free();
		GuiRenderer::free();
		delete window;

		SDL_Quit();
	}
	void createWindow(int w = 1080, int h = 720, char* title = "OPENGL WINDOW") {
		window = new Display(w, h, title);
	}
	void init() {
		Render::init();
		GuiRenderer::init();
	}
	inline void update() {
		//AmbientLight::getAmbient()->osciling();
		FPSCounter::count();
		window->update();
	}
	inline bool getCloseRequest() {
		update();
		return window->getCloseRequest();
	}


};