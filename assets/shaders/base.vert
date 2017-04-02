#version 440 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec3 vertColor;
layout (location = 3) in vec2 vertUV;
layout (location = 4) in vec3 vertTangent;

out vec3 vPos;
out vec3 vNormal;
out vec3 vColor;
out vec2 vUV;
out vec3 vTangent;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main() {
	vPos = vertPos;
	vNormal = vertNormal;
	vColor = vertColor;
	vUV = vertUV;
	vTangent = vertTangent;

	gl_Position = p * v * m * vec4(vPos, 1);
}
