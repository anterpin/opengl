 #version 430

in vec2 passCoord ;
in flat int passPage;

layout(location=0)out vec4 outColor;

uniform sampler2D atlas[3];
uniform vec3 color;

uniform float width;
uniform float edge;

uniform float borderWidth;
uniform float borderEdge;
uniform vec2 offset;
uniform vec3 outLineColor;


void main(void){

	float distance = 1.0 - texture(atlas[passPage],passCoord).a;
	float alpha = 1.0 - smoothstep(width,width+edge,distance);

	float distance2 = 1.0 - texture(atlas[passPage],passCoord+offset).a;
	float outLineAlpha = 1.0 - smoothstep(borderWidth,borderWidth+borderEdge,distance2);

	float overAllAlpha = alpha + (1.0 - alpha)*outLineAlpha;
	vec3 overallColor = mix(outLineColor,color,alpha/overAllAlpha);
	

	outColor=vec4(overallColor,overAllAlpha);

}