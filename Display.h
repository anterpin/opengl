#pragma once
#include "MasterRenderer.h"
#include "Input.h"

#include <Windows.h>
#include <iostream>
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <assert.h>



class Display {
private:
#define targetMaiorVersion  4
#define targetMinorVersion  3
#define winMode  SDL_WINDOW_OPENGL

	SDL_Window* window = nullptr;
	SDL_GLContext glContext;
	
	int windowID = 0;
	bool closeRequest = false;


	void inputHandle();
	
	void InitGlew();
	void close();
public:
	Display(int w = 1080,int h = 720,char* name = "OPENGL WINDOW");
	static Display* getWindow();
	~Display();
	void getSize(int& width,int& height);
	bool getCloseRequest();
	void update();
	void getSize(float& width, float& height);
	

};