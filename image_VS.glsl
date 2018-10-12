#version 430

in vec2 position;
in vec2 textCoords;

out vec2 passTextCoords;

uniform mat4 trasform;
uniform bool invertedTex;

void main(){
	passTextCoords.x = textCoords.x;
	if(invertedTex){
		passTextCoords.y = 1-textCoords.y;
	}
	else{
		passTextCoords.y = textCoords.y;
	}
	
	gl_Position=trasform*vec4(position,0,1);
}
