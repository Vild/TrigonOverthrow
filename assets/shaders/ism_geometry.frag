#version 440 core
layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_normal;

layout (location = 0) out vec3 o_position;
layout (location = 1) out vec3 o_normal;
layout (location = 2) out vec4 o_diffuseSpec;

uniform vec4 u_diffuseSpec;

void main() {
	o_position = i_position;
	o_normal = i_normal;
	o_diffuseSpec = u_diffuseSpec;
}
