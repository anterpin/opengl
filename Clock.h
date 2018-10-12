#pragma once
#include <ctime>
#include <chrono>

class Clock {
	long time = 0;
	long prevTime = 0;
public:
	static Clock* getInstance() {
		static Clock* clock = nullptr;
		if (clock == nullptr) {
			clock = new Clock();
		}
		return clock;
	}
	void updateClock() {
		prevTime = time;
		time = clock(); //std::chrono::high_resolution_clock::now();
		
	}
	float getSpaceInTime() {
		// 1 m/s
		long deltaTime = time - prevTime;
		return deltaTime / 1000.0f;
	}
	float getSeconds() {
		// 1 m/s
		return  time/1000.0f;
	}
};