#version 430 core

const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 4;

in vec3 position;
in vec2 uv;
in vec3 normal;

in ivec4 jointIds;
in vec4 weights;

out vec2 passUv_GS;
out vec3 passNormal_GS;


uniform mat4 jointTrasforms[MAX_JOINTS];

uniform bool hasAnimation;
uniform mat4 trasformMatrix;
uniform vec4 plane;


void main(void){

	vec4 worldPosition = vec4(0,0,0,0);
	vec4 totalNormal = vec4(0,0,0,0);
	

	if (hasAnimation) {
		for (int i = 0; i<MAX_WEIGHTS; i++) {
			mat4 jointTransform = jointTrasforms[jointIds[i]];
			vec4 posePosition = jointTransform * vec4(position, 1.0);
			worldPosition += posePosition * weights[i];

			vec4 worldNormal = jointTransform * vec4(normal, 0.0);
			totalNormal += worldNormal * weights[i];
		}
	}
	else {
		worldPosition = vec4(position, 1);
		totalNormal = vec4(normal, 0);
	}

	worldPosition = trasformMatrix*worldPosition;
	passNormal_GS = normalize((trasformMatrix*totalNormal).xyz);

	gl_ClipDistance[0] = dot(worldPosition, plane);
	

	gl_Position = worldPosition;
	passUv_GS = uv;

}
