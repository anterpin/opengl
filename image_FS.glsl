#version 430 
#include "Camera.glh"
#include "Multisample.glh"

layout(location=0) out vec4 outColor;

in vec2 passTextCoords;

uniform sampler2DMS imageMs;
uniform int msaa;

uniform bool useMsaa;

uniform sampler2D image;
uniform float trasp;

uniform bool linealizeDepth;

float LinearizeDepth(float depth){
    float zNear = camera.zNear;    // TODO: Replace by the zNear of your perspective projection
    float zFar  = camera.zFar; // TODO: Replace by the zFar  of your perspective projection
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

void main(){
	
	vec4 col;
	if(useMsaa){
		col = texelFetch(imageMs,ivec2(passTextCoords*vec2(textureSize(imageMs))),msaa);
	}
	else{
		col=texture(image,passTextCoords);
	}

	if(linealizeDepth){
		float v = col.x;
		v = LinearizeDepth(v);
		col.x = v;
		col.y = v;
		col.z = v;
	}

	col.a=trasp;

	outColor=col;
}