#version 440 core
// NOTE: The locations must match ShaderAttributeID
layout(location = 12) in vec3 vertPos;
layout(location = 13) in vec3 vertVel;
layout(location = 14) in float life;
layout(location = 15) in vec3 vertColor;

out vec4 vColor;

uniform mat4 v;
uniform mat4 p;

void main() {
	vColor = vec4(vertColor, life);
	gl_PointSize = 2;
	gl_Position = p * v * vec4(vertPos, 1.0f);
}