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

uniform vec3 particleCenter;
uniform vec3 cameraRight_wPos;
uniform vec3 cameraUp_wPos;
// uniform vec3 billboardSize; Might hardcode it.
uniform vec3 cameraPos;
uniform mat4 viewProj;

void main() {
	vPos = particleCenter + (vec3(cameraRight_wPos * vertPos.x * 2) + vec3(cameraUp_wPos * vertPos.y * 2));
	vNormal = cameraPos - vPos;
	vColor = vertColor;
	vUV = vertUV;

	gl_Position = viewProj * m * vec4(vPos, 1.0f);
}
