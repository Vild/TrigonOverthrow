#version 440 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout (location = 0) in vec3 i_position[];

layout (location = 0) out vec3 o_position;
layout (location = 1) out vec3 o_normal;

uniform mat4 u_view;
uniform mat4 u_projection;

vec3 calcNormal() {
	vec3 p0 = i_position[0].xyz;
	vec3 p1 = i_position[1].xyz;
	vec3 p2 = i_position[2].xyz;

	vec3 edge0 = p1 - p0;
	vec3 edge1 = p2 - p0;

	vec3 normal = cross(edge0, edge1);

	return normalize(normal);
}

void main() {
	o_normal = calcNormal();
	for (int i = 0; i < 3; i++) {
		o_position = i_position[i];
		gl_Position = u_projection * u_view * vec4(o_position, 1);
		EmitVertex();
	}

	EndPrimitive();
}
