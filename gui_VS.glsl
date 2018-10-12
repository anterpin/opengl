#version 430

layout(location = 0)in vec2 position;
in vec2 textCoord;
in int page;

out vec2 passCoord ;
out flat int passPage;


uniform mat4 trasform;


void main(void){
	vec2 screenPosition=(trasform*vec4(position,0,1)).xy;

	passCoord=textCoord;
	passPage=page;

	gl_Position = vec4(screenPosition,0,1);


}