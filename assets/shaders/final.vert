#version 430 core

// NOTE: The locations must match ShaderAttributeID
layout (location = 0) in vec3 vertPos;
layout (location = 2) in vec3 vertColor;
layout (location = 3) in vec2 vertUV;
layout (location = 5) in mat4 m;

out vec3 vColor;
out vec2 vUV;

uniform mat4 vp;

void main() {
	vec4 pos = m * vec4(vertPos, 1.0f);

	vColor = vertColor;
	vUV = vertUV;

	gl_Position = vp * pos;
}
