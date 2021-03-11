#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


uniform vec3 lightPos;

//out vec4 fragPosLightSpace;
//uniform mat4 lightSpaceTrMatrix;

out vec4 fragPosLightSpace;
uniform mat4 lightSpaceTrMatrix;
uniform int fog;
uniform int pornesteLuminaDir;
uniform int fulger;
uniform int pornirePloaie;

out vec4 fPosEye;
out vec4 lightPosEye;


void main() 
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
	fPosition = vPosition;
	fNormal = vNormal;
	fTexCoords = vTexCoords;
	fragPosLightSpace = lightSpaceTrMatrix * model * vec4(vPosition, 1.0f);
	
	
	fPosEye = view * model * vec4(fPosition, 1.0f);
    lightPosEye = view * model * vec4(lightPos, 1.0f);
}
