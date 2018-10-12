#version 400 core

in vec2 passUv;
in vec3 passNormal;

out vec4 out_Color;

uniform sampler2D colorTexture;
uniform vec3 ambient;

const vec3 lightDirection= vec3(0.0,0.0,-1.0);
const float intensity = 0.9;

float diffuse(vec3 direction,vec3 normal,float intensity){
	return max(0.2,dot(normal,-direction)*intensity);
}

void main(void){

vec4 totalLight = vec4(ambient,1);

vec4 cor=texture(colorTexture,passUv);



out_Color = cor*diffuse(lightDirection, passNormal, intensity)*totalLight;




}