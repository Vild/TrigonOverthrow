#version 440 core

out vec4 outColor;

in vec3 gColor;
in vec2 gUV;

uniform sampler2D fontMap;

void main() {
	vec4 charTex = texture(fontMap, gUV);
	if (charTex.a < 0.5)
		discard;
	outColor = vec4(charTex.xyz * gColor, 1);
}
