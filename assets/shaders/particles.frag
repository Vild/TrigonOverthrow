#version 440 core

in vec3 vPos;
in vec3 vNormal;
in vec3 vColor;
in vec2 vUV;

out vec4 outColor;


void main() {
	outColor = vec4(vColor, 1.0);
}
