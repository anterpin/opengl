#include "Display.h"

#define GLEW_STATIC
#define VSYNC 0

static Display* m_display = nullptr;

Display* Display::getWindow() {
	return m_display;
}
Display::Display(int w,int h,char* name) {

	m_display = this;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	window = SDL_CreateWindow(
		name,                  // window title
		SDL_WINDOWPOS_CENTERED,           // initial x position
		SDL_WINDOWPOS_CENTERED,           // initial y position
		w,                               // width, in pixels
		h,                               // height, in pixels
		winMode                  // flags - see below
	);
	if (!window) {
		std::cerr << "Could not create window:" << SDL_GetError() << std::endl;
		assert(false);
	}

	windowID = SDL_GetWindowID(window);
	
	glContext = SDL_GL_CreateContext(window);
	
	if (!glContext){
		// Display error message
		std::cerr << "OpenGL context could not be created! SDL Error:" << SDL_GetError() << std::endl;
		assert(false);
	}
	SDL_GL_SetSwapInterval(VSYNC);
	InitGlew();

};
void Display::getSize(float& width, float& height) {
	int w, h;
	this->getSize(w, h);
	height = (float)h;
	width = (float)w;
};
Display::~Display() {
	close();
}

void Display::InitGlew() {
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	int supportedMaiorVersion;
	int supportedMinorVersion;
	glGetIntegerv(GL_MAJOR_VERSION, &supportedMaiorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &supportedMinorVersion);
	if (supportedMaiorVersion < targetMaiorVersion) {
		std::cerr << "Opengl version not compatible"<<std::endl;
		assert(false);
	}
	else if (supportedMaiorVersion == targetMaiorVersion && supportedMinorVersion < targetMinorVersion) {
		std::cerr << "Opengl version not compatible" << std::endl;
		assert(false);
	}
		
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, targetMaiorVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, targetMinorVersion);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	glewExperimental = GL_TRUE;
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		assert(false);
	}	
}

void Display::close() {
	SDL_GL_DeleteContext(this->glContext);
	SDL_DestroyWindow(this->window);
}
void Display::getSize(int& width,int& height) {
	SDL_GetWindowSize(this->window, &width, &height);
}
void Display::update() {
	
	SDL_GL_SwapWindow(window);
	if (Input::isKeyDownRepeat(SDLK_l)) {
		RenderUtils::changeWireFrame();
	}

	inputHandle();
}
bool Display::getCloseRequest() {
	return closeRequest;
}
void Display::inputHandle() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		
		switch (event.type) {
			
			case SDL_WINDOWEVENT:
				if (event.window.windowID == this->windowID) {
					if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
						close();
						closeRequest = true;
					}
				}
				break;
			case SDL_QUIT:
				//closeRequest = true;
				break;
			case SDL_KEYDOWN:
				Input::keyDown(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				Input::keyUp(event.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				if (event.motion.state & SDL_BUTTON_LMASK){
					Input::cursorDeltaPos(event.motion.xrel, event.motion.yrel);
				}
				break;
			default:
				break;
		}
	}
	
	
}

