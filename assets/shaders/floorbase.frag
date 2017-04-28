#version 440 core

// Deferred output
layout (location = 0) out vec3 defPos;
layout (location = 1) out vec3 defNormal;
layout (location = 2) out vec4 defDiffuseSpecular;

in vec3 gPos;
in vec3 gNormal;
in vec3 gColor;
in vec2 gUV;
in mat3 gTBN;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform float setting_defaultSpecular;

void main() {
	defPos = gPos;

	vec3 normal = normalize(texture2D(normalTexture, gUV).rgb * 2.0 - vec3(1.0));
	defNormal = normalize(gTBN * normal);

	defDiffuseSpecular.rgb = vec3(1, 1, 1);
	defDiffuseSpecular.a = setting_defaultSpecular;
}
