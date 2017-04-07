#version 430 core

// NOTE: The locations must match ShaderAttributeID
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec3 vertColor;
layout (location = 3) in vec2 vertUV;
layout (location = 4) in vec3 vertTangent;
layout (location = 5) in mat4 m;

out vec3 vPos;
out vec3 vNormal;
out vec3 vColor;
out vec2 vUV;
out vec3 vTangent;
out mat4 vM;

void main() {
	vPos = vertPos;
	vNormal = vertNormal;
	vColor = vertColor;
	vUV = vertUV;
	vTangent = vertTangent;
	vM = m;
}
