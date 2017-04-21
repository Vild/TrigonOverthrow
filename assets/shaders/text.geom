#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in vec3 vPos[];
in vec3 vColor[];
in vec2 vUV[];
in vec4 vCharRect[];
in vec3 vCharPos[];

out vec3 gColor;
out vec2 gUV;

uniform mat4 m; // m will not change foreach char
uniform mat4 vp;

void main() {
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 3; i++) {
			gColor = vColor[i] * (1 - j * 0.75);
			// vUV      = [0..1, 0..1]
			// vCharRect.xy = [startX, startY] in texture
			// vCharRect.zw = [width, height] of char
			// vCharPos     = [xPos, yPos]
			gUV = vUV[i] * vCharRect[i].zw + vCharRect[i].xy;

			gl_Position = vp * m * (vec4(vPos[i] * vec3(vCharRect[i].zw, 1) + vCharPos[i], 1) + vec4(j * -0.01, j * 0.005, j * 0.01, 0));
			EmitVertex();
		}
		EndPrimitive();
	}
}
