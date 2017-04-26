#version 440 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout (location = 0) in vec3 i_position[];

layout (location = 0) out vec3 o_position;
layout (location = 1) out vec3 o_normal;

uniform mat4 u_view;
uniform mat4 u_projection;

vec3 calcNormal()
{
  return vec3(0, 1, 0);
}

void main()
{
  o_normal = calcNormal();
  for (int i = 0; i < 3; i++)
  {
    o_position = i_position[i];
    gl_Position = u_projection * u_view * vec4(o_position, 1);
    EmitVertex();
  }

  EndPrimitive();
}
