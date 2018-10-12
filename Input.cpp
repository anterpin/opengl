#include "Input.h"
#include <vector>

static InputDevice device1;

int Input::getDX() {
	return device1.getDX();
}
int Input::getDY() {
	return device1.getDY();
}
void Input::keyDown(unsigned int key){
	device1.keyDown(key);
}
void Input::keyUp(unsigned int key) {
	device1.keyUp(key);
}
bool Input::isKeyDown(unsigned int key) {
	return device1.isKeyDown(key);
}
bool Input::isKeyDownRepeat(unsigned int key) {
	return device1.isKeyDownRepeat(key);
}
void Input::cursorPos(unsigned int x, unsigned int y) {
	device1.cursorPos(x, y);
}
void Input::cursorDeltaPos(int x, int y) {
	device1.cursorDeltaPos(x, y);
}

