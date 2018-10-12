#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "AnimatedModel.h"

class AnimatedObject : public GameObject {
	float animationTime = 0;

	std::vector<JointTrasform> arr;
	std::vector<glm::mat4> jointTrasform;
public:
	AnimatedObject(Model* m) :GameObject(m) {
		arr.resize(((AnimatedModel*)m)->getAnimation()->jointAnimation.size());
		jointTrasform.resize(((AnimatedModel*)m)->getAnimation()->jointAnimation.size());
	}
	std::vector<glm::mat4>* getJointTrasform() {
		for (Joint* j : ((AnimatedModel*)model)->getArmature()->children) {
			updateToArr(j);
		}
		return &jointTrasform;
	}
	void updateToArr(Joint* joint) {
		jointTrasform[joint->index] = joint->animatedTrasform;
		for (Joint* j : joint->children) {
			updateToArr(j);
		}
	}
	void update()override {
		increaseAnimationTime();
		udpateJT();

		for (Joint* j : ((AnimatedModel*)model)->getArmature()->children) {
			applyPose(j, glm::mat4(1.0f));
		}

	}
	bool isAnimated()override {
		return true;
	}
	void applyPose(Joint* joint, const glm::mat4& parentTrasform) {

		glm::mat4 currentLoc = arr[joint->index].getLocalTrasform();
		glm::mat4 currentTra = parentTrasform * currentLoc;
		for (Joint* j : joint->children) {
			applyPose(j, currentTra);
		}
		joint->animatedTrasform = currentTra * joint->inverseBindMat;

	}
	void increaseAnimationTime() {
		animationTime += Clock::getInstance()->getSpaceInTime();

		if (animationTime > ((AnimatedModel*)model)->getAnimation()->length) {
			int timesBigger = (int)(animationTime / ((AnimatedModel*)model)->getAnimation()->length);
			animationTime -= timesBigger*((AnimatedModel*)model)->getAnimation()->length;// c - (int)c;
		}
		//animationTime = 0.5f;
	}
	float calculateProgression(KeyFrame* a, KeyFrame* b) {
		float betweenTime = b->time - a->time;
		float currentTime = animationTime - a->time;
		return currentTime / betweenTime;

	}
	void udpateJT() {
		for (int i = 0; i < ((AnimatedModel*)model)->getAnimation()->jointAnimation.size(); i++) {
			JointAnimation& jA = ((AnimatedModel*)model)->getAnimation()->jointAnimation[i];
			KeyFrame a, b;

			jA.getPrevAndNextKF(a, b, animationTime);

			float progr = calculateProgression(&a, &b);


			arr[i] = JointTrasform::interpolate(a.trasform, b.trasform, progr);
			glm::mat4 m = arr[i].getLocalTrasform();


		}
	}


};