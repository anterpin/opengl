#pragma once
#include <gainput\gainput.h>
#include "Display.h"

static gainput::InputManager manager;
static gainput::InputMap* map;
enum Button
		{
			ButtonConfirm
		};
struct InputDevice{
	gainput::InputManager* manager;
	gainput::DeviceId keyboardId;
	gainput::DeviceId mouseId;
	gainput::DeviceId padId;
	gainput::DeviceId touchId;
	InputDevice(gainput::InputManager* manager) {
		this->manager = manager;
		keyboardId = manager->CreateDevice<gainput::InputDeviceKeyboard>();
		mouseId = manager->CreateDevice<gainput::InputDeviceMouse>();
		padId = manager->CreateDevice<gainput::InputDevicePad>();
		touchId = manager->CreateDevice<gainput::InputDeviceTouch>();
	}
};
namespace GInput {
	void init() {
		

		gainput::InputManager manager;
		float w, h;
		Display::getWinSize(w, h);
		manager.SetDisplaySize(w, h);
		map = new gainput::InputMap(manager);

	}
	__forceinline void update() {
		manager.Update();

		// May need some platform-specific message handling here

		if (map->GetBoolWasDown(ButtonConfirm))
		{
			// Confirmed!
		}
	}
}