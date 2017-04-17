#version 440 core

out vec4 outColor;

in vec3 vColor;
in vec2 vUV;

uniform sampler2D fontMap;

void main() {
	vec4 charTex = texture(fontMap, vUV);
	if (charTex.a < 0.5)
		discard;
	outColor = vec4(charTex.xyz * vColor, 1);
}
