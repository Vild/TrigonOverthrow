#version 440 core

in vec3 vPos;
in vec3 vNormal;
in vec3 vColor;
in vec2 vUV;

out vec4 outColor;

uniform sampler2D diffuseTexture;

void main() {
	// Sample texture
	outColor = vColor;
}
