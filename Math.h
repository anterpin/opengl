#pragma once
#include <GLM\glm.hpp>
#include <GLM\gtx\quaternion.hpp>
#include<GLM/gtx/transform.hpp>



using namespace glm;

namespace math {
	static void NormalizePlane(glm::vec4& plane)
	{
		float mag;
		mag = glm::sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
		plane.x /= mag;
		plane.y /= mag;
		plane.z /= mag;
		plane.w /= mag;
	}
	static quat eulerToQuat(const vec3& rot) {
		vec3 r(rot.x, rot.y, rot.z);
		//glm::quat& q = glm::angleAxis(glm::radians(90.f), r);
		glm::quat q(r);
		return q;
	}
	static vec3 quatToEuler(const quat& rot) {
		vec3 v = glm::eulerAngles(rot);
		return v;
	}
	static vec3 toRad(const vec3& v) {
		vec3 r(glm::radians(v.x), glm::radians(v.y), glm::radians(v.z));
		return r;
	}
	static mat4 Trasl(const vec3& pos) {
		mat4 m(1.0f);
		m[3][0] = pos.x;
		m[3][1] = pos.y;
		m[3][2] = pos.z;
		return m;
	}
	static mat4 Scal(const vec3& scal) {
		mat4 m(1.0f);
		m[0][0] = scal.x;
		m[1][1] = scal.y;
		m[2][2] = scal.z;
		return m;
	}
	static mat4 Rot(const quat& rot) {
		mat4 m(1.0f);
		m = glm::toMat4(rot);
		return m;
	}

	static bool insideFrustumAABB(
		const glm::vec4* planes,
		const glm::vec3& max,
		const glm::vec3& min){
		// Indexed for the 'index trick' later
		glm::vec3 box[2] = { min, max };


		// We only need to do 6 point-plane tests
		for (int i = 0; i < 6; ++i)
		{
			// This is the current plane
			const glm::vec4 &p = planes[i];

			// p-vertex selection (with the index trick)
			// According to the plane normal we can know the
			// indices of the positive vertex
			const int px = static_cast<int>(p.x > 0.0f);
			const int py = static_cast<int>(p.y > 0.0f);
			const int pz = static_cast<int>(p.z > 0.0f);

			// Dot product
			// project p-vertex on plane normal
			// (How far is p-vertex from the origin)
			const float dp =
				(p.x*box[px].x) +
				(p.y*box[py].y) +
				(p.z*box[pz].z);

			// Doesn't intersect if it is behind the plane
			if (dp < -p.w) { return false; }
		}
		return true;
	}
	static int intersectFrustumAABB(
		const glm::vec4* planes,
		const glm::vec3& max,
		const glm::vec3& min) {//0 out ; 1 in // -1 intersect
		// Indexed for the 'index trick' later
		glm::vec3 box[2] = { min, max };
		// We only need to do 6 point-plane tests
		for (int i = 0; i < 6; ++i)
		{
			// This is the current plane
			const glm::vec4 &p = planes[i];

			// p-vertex selection (with the index trick)
			// According to the plane normal we can know the
			// indices of the positive vertex
			const int px = static_cast<int>(p.x > 0.0f);
			const int py = static_cast<int>(p.y > 0.0f);
			const int pz = static_cast<int>(p.z > 0.0f);

			// Dot product
			// project p-vertex on plane normal
			// (How far is p-vertex from the origin)
			const float dp =
				(p.x*box[px].x) +
				(p.y*box[py].y) +
				(p.z*box[pz].z);

			const float dv =
				(p.x*box[1-px].x) +
				(p.y*box[1-py].y) +
				(p.z*box[1-pz].z);

			// Doesn't intersect if it is behind the plane
			if (dv < -p.w) {
				if (dp < -p.w) {
					return 0;
				}
				return -1;
			}
		}
		return 1;
	}
	static int leftRotate(int n, unsigned int d) {
		return (n << d) | (n >> (sizeof(int)*8 - d));
	}
	static int reverseBits(int input){
		int output = input;
		for (int i = sizeof(input) * 8 - 1; i; --i)
		{
			output <<= 1;
			input >>= 1;
			output |= input & 1;
		}
		return output;
	}
	static void ExtractPlanes(
		glm::vec4* p_planes,
		const glm::mat4& comboMatrix,
		bool normalize)
	{
		// Left clipping plane
		p_planes[0].x = comboMatrix[0][3] + comboMatrix[0][0];
		p_planes[0].y = comboMatrix[1][3] + comboMatrix[1][0];
		p_planes[0].z = comboMatrix[2][3] + comboMatrix[2][0];
		p_planes[0].w = comboMatrix[3][3] + comboMatrix[3][0];
		// Right clipping plane
		p_planes[1].x = comboMatrix[0][3] - comboMatrix[0][0];
		p_planes[1].y = comboMatrix[1][3] - comboMatrix[1][0];
		p_planes[1].z = comboMatrix[2][3] - comboMatrix[2][0];
		p_planes[1].w = comboMatrix[3][3] - comboMatrix[3][0];
		// Top clipping plane
		p_planes[2].x = comboMatrix[0][3] - comboMatrix[0][1];
		p_planes[2].y = comboMatrix[1][3] - comboMatrix[1][1];
		p_planes[2].z = comboMatrix[2][3] - comboMatrix[2][1];
		p_planes[2].w = comboMatrix[3][3] - comboMatrix[3][1];
		// Bottom clipping plane
		p_planes[3].x = comboMatrix[0][3] + comboMatrix[0][1];
		p_planes[3].y = comboMatrix[1][3] + comboMatrix[1][1];
		p_planes[3].z = comboMatrix[2][3] + comboMatrix[2][1];
		p_planes[3].w = comboMatrix[3][3] + comboMatrix[3][1];
		// Near clipping plane
		p_planes[4].x = comboMatrix[0][2];
		p_planes[4].y = comboMatrix[1][2];
		p_planes[4].z = comboMatrix[2][2];
		p_planes[4].w = comboMatrix[3][2];
		// Far clipping plane
		p_planes[5].x = comboMatrix[0][3] - comboMatrix[0][2];
		p_planes[5].y = comboMatrix[1][3] - comboMatrix[1][2];
		p_planes[5].z = comboMatrix[2][3] - comboMatrix[2][2];
		p_planes[5].w = comboMatrix[3][3] - comboMatrix[3][2];
		// Normalize the plane equations, if requested
		if (normalize == true)
		{
			NormalizePlane(p_planes[0]);
			NormalizePlane(p_planes[1]);
			NormalizePlane(p_planes[2]);
			NormalizePlane(p_planes[3]);
			NormalizePlane(p_planes[4]);
			NormalizePlane(p_planes[5]);
		}
	}
}