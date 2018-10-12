#version 430 

layout(location=0) out vec4 outColor;

uniform bool intersect;

void main(){
	if(intersect){
		outColor=vec4(1,0,0,1);
	}
	else{
		outColor=vec4(0,1,0,1);
	}
	
}