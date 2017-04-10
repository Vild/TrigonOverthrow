#version 440 core

// NOTE: The locations must match ShaderAttributeID
layout (location = 0) in vec3 vertPos;
layout (location = 2) in vec3 vertColor;
layout (location = 3) in vec2 vertUV;

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
uniform sampler2D particlePos;

void main() {
	// Particle center is always in origin from the beginning.
	vNormal = cameraPos - vPos;
	vPos = vertPos;
	vColor = vertColor;
	vUV = vertUV;

	gl_Position = p * v * vec4(vPos, 1.0f);
}
