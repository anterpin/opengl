#pragma once
#include "Math.h"

struct JointTrasform {
#define column_Major true
	glm::vec3 position;
	glm::quat rotation;
	
private:
	static glm::vec3 interpolate(const glm::vec3& v1, const glm::vec3& v2, float progr) {
		return  v1 + (v2 - v1)*progr;
	}
	static glm::quat interpolateQuat(const glm::quat& a, const glm::quat& b, float blend) {
		glm::quat result(1, 0, 0, 0);
		float dot = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
		float blendI = 1.0f - blend;
		if (dot < 0) {
			result.w = blendI * a.w + blend * -b.w;
			result.x = blendI * a.x + blend * -b.x;
			result.y = blendI * a.y + blend * -b.y;
			result.z = blendI * a.z + blend * -b.z;
		}
		else {
			result.w = blendI * a.w + blend * b.w;
			result.x = blendI * a.x + blend * b.x;
			result.y = blendI * a.y + blend * b.y;
			result.z = blendI * a.z + blend * b.z;
		}
	
		return glm::normalize(result);
	}
public:
	void setFromMatrix(const glm::mat4& mat) {
		for (int i = 0; i < 3; i++) {
			position[i] = !column_Major ? mat[i][3] : mat[3][i];
		}
		rotation = glm::toQuat(mat);
	}

	glm::mat4 getLocalTrasform() {
		return math::Trasl(position)*math::Rot(rotation);
	}
	JointTrasform interpolate(const JointTrasform& frameB, float progression) {
		return interpolate(*this, frameB, progression);
	}
	static JointTrasform interpolate(const JointTrasform& frameA, const JointTrasform& frameB, float progression) {
		JointTrasform j;
		j.position = interpolate(frameA.position, frameB.position, progression);
		j.rotation = interpolateQuat(frameA.rotation,frameB.rotation,progression);
		
		return j;
	}

};