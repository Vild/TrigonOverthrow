#version 440 core
layout (location = 0) in vec3 i_position;
layout (location = 1) in mat4 i_m;

uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
  gl_Position = u_projection * u_view * i_m * vec4(i_position, 1);
}
