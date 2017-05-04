#version 440 core
layout(location = 0) in vec3 position;
layout(location = 0) out vec2 uv;

void main() {
	gl_Position = vec4(position, 1);
	uv = vec2(position) * 0.5 + 0.5;
}
