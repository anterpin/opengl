#pragma once
#include "KeyFrame.h"

struct JointAnimation {

	std::vector<KeyFrame> keyFrames;

	void getPrevAndNextKF(KeyFrame& prev, KeyFrame& next,float animTime) {

		prev = keyFrames[0];
		next = keyFrames[0];
		

		for (int i = 1; i < keyFrames.size();i++) {
			next = keyFrames[i];
			if (next.time > animTime) {
				return;
			}
			prev = keyFrames[i];
		}


	}
};
