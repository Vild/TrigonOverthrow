#version 440 core
layout (location = 0) in vec3 i_position;
layout (location = 1) in mat4 i_m;

layout (location = 0) out vec3 o_position;

void main()
{
  o_position = vec3(i_m * vec4(i_position, 1));
}
