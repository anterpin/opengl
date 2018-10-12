#version 430

in vec3 position;

uniform mat4 trasformMat;
#include "Camera.glh"

uniform vec4 plane;

void main(){

	vec4 worldPosition = trasformMat*vec4(position,1);
	gl_ClipDistance[0] = dot(worldPosition,plane);

	gl_Position=camera.projectViewMat * worldPosition ;
}