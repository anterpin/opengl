#pragma once
#include <Windows.h>
#include <GLM/glm.hpp>
#include <stdio.h>
struct Point {
	unsigned int x ;
	unsigned int y ;
	void print() {
		printf("%d %d \n", x, y);
	}
};
#define NUM_KEYS 255
struct InputDevice{
	unsigned int id=0;
	bool keys[NUM_KEYS] = { 0 };
	bool keysDown[NUM_KEYS] = { 0 };
	Point cursor_pos;
	Point deltaPos;
	void cursorPos(unsigned int x, unsigned int y) {
		cursor_pos.x = x;
		cursor_pos.y = y;
	}
	void cursorDeltaPos(int x, int y) {
		deltaPos.x = x;
		deltaPos.y = y;
	}
	int getDX() {
		int deltaX = deltaPos.x;
		deltaPos.x = 0;
		return deltaX;
	}
	int getDY() {
		int deltaY = deltaPos.y;
		deltaPos.y = 0;
		return deltaY;
	}
	void keyDown(unsigned int key) {
		if (key > NUM_KEYS - 1)
			return;
		bool& k = keys[key];
		if (!k)
			k = true;
	}
	void keyUp(unsigned int key) {
		if (key > NUM_KEYS - 1)
			return;
		bool& k = keys[key];

		if (k)
			k = false;
	}
	bool isKeyDown(unsigned int key) {
		if (key > NUM_KEYS - 1)
			return false;
		return keys[key];
	}
	bool isKeyDownRepeat(unsigned int key) {
		if (key > NUM_KEYS - 1)
			return false;
		bool& down = keys[key];
		bool& alreadyDown = keysDown[key];		
	
		if (down && !alreadyDown) {
			alreadyDown = true;
			
			return true;
		}
		if (!down&&alreadyDown) {
			alreadyDown = false;
		}		
		
		return false;
	}

};


class Input
{
public:
	static void cursorPos(unsigned int x, unsigned int y);
	static int getDX();
	static int getDY();
	static void keyDown(unsigned int key);
	static void keyUp(unsigned int key);
	static bool isKeyDown(unsigned int key);
	static bool isKeyDownRepeat(unsigned int key);
	static void cursorDeltaPos(int x, int y);
	

};

