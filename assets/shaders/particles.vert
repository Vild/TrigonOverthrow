#version 440 core

// NOTE: The locations must match ShaderAttributeID
layout (location = 0) in vec3 vertPos;
layout (location = 2) in vec3 vertColor;
layout (location = 3) in vec2 vertUV;
layout (location = 5) in mat4 m;

out vec3 vPos;
out vec3 vNormal;
out vec3 vColor;
out vec2 vUV;

uniform vec3 cameraRight_wPos;
uniform vec3 cameraUp_wPos;
uniform float billboardSize; //Might hardcode it.
uniform vec3 cameraPos;
uniform mat4 v;
uniform mat4 p;

void main() {
	// Particle center is always in origin from the beginning.
	vNormal = vec3(0, 1, -2) - vPos;
	vPos = vec3(0,0,0) + (vec3(cameraRight_wPos * vertPos.x * billboardSize) + vec3(cameraUp_wPos * vertPos.y * billboardSize));
	vColor = vertColor;
	vUV = vertUV;

	gl_Position = p * v * m * vec4(vPos, 1.0f);
}
