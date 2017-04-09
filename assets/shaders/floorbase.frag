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

in flat vec4 gTopY;
in flat vec4 gBottomY;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform float setting_defaultSpecular;

void main() {
	defPos = gPos;

	vec3 normal = normalize(texture2D(normalTexture, gUV).rgb * 2.0 - vec3(1.0));
	defNormal = normalize(gTBN * normal);

	vec3 fragColor;


	float r = gBottomY.y + 0.05;
	float b = gTopY.y - 0.05;

	float rAmount = clamp(1 - abs(gPos.y - r) * 40, 0, 1) * 100;
	float bAmount = clamp(1 - abs(gPos.y - b) * 40, 0, 1) * 100;

	defDiffuseSpecular.a = setting_defaultSpecular;

	fragColor = vec3(1, 1, 1);
	if (gPos.y >= gTopY.y - 0.01)
		fragColor = mix(fragColor, vec3(0, 0, 0), 1 - abs(gTopY.x - gPos.x) - abs(gTopY.z - gPos.z));
	fragColor = mix(fragColor, vec3(1, 0, 0), rAmount);
	fragColor = mix(fragColor, vec3(0, 0, 1), bAmount);
	defDiffuseSpecular.rgb = fragColor;
}
