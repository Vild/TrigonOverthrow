#version 440 core

in vec2 vUV;

out vec4 fragColor;

uniform sampler2D diffuseTexture;

void main() {
	fragColor = texture(diffuseTexture, vUV);
}
