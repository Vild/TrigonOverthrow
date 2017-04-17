#version 440 core
// NOTE: The locations must match ShaderAttributeID
layout (location = 0) in vec3 vertPos;
layout (location = 2) in vec3 vertColor;
layout (location = 3) in vec2 vertUV;
layout (location = 9) in vec4 charRect;
layout (location = 10) in vec3 charPos;

out vec3 vPos;
out vec3 vColor;
out vec2 vUV;
out vec4 vCharRect;
out vec3 vCharPos;

void main() {
	vPos = vertPos;
	vColor = vertColor;
	vUV = vertUV;
	vCharRect = charRect;
	vCharPos = charPos;
}
