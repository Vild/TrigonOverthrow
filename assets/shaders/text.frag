#version 440 core

out vec4 outColor;

in vec3 vColor;
in vec2 vUV;

uniform sampler2D fontMap;

void main() {
	vec3 charTex = texture(fontMap, vUV).xyz;

	outColor = vec4(charTex * vColor, 1);
}
