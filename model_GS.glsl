#version 430

#include "Camera.glh"
layout(triangles) in;
layout(triangle_strip, max_vertices = 9) out;

in vec2 passUv_GS[];
in vec3 passNormal_GS[];

out vec2 passUv;
out vec3 passNormal;

void main() {

	for (int i = 0; i < gl_in.length(); i++) {
		gl_Position = camera.projectViewMat * gl_in[i].gl_Position;
		passUv = passUv_GS[i];
		passNormal = passNormal_GS[i];
		EmitVertex();
	}
	EndPrimitive();

}
