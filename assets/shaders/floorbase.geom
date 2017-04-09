#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vPos[];
in vec3 vNormal[];
in vec3 vColor[];
in vec2 vUV[];
in vec3 vTangent[];
in mat4 vM[];
in float vTop[];
in vec4 vNeighbor[];

out vec3 gPos;
out vec3 gNormal;
out vec3 gColor;
out vec2 gUV;
out mat3 gTBN;

out flat vec4 gTopY;
out flat vec4 gBottomY;

uniform mat4 v;
uniform mat4 p;
uniform vec3 cameraPos;

uniform bool setting_doBackFaceCulling;

#define M_PI 3.1415

mat3 calcTBN(mat3 normalMatrix, vec3 normal, int idx) {
	vec3 T = normalize(normalMatrix * vTangent[idx]);
	vec3 N = normalize(normalMatrix * normal);
	// Gram-Schmidt process
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T, N);
	return mat3(T, B, N);
}

void main() {
	int i;

	vec3 p0 = (vM[0] * vec4(vPos[0], 1.0f)).xyz;
	vec3 p1 = (vM[1] * vec4(vPos[1], 1.0f)).xyz;
	vec3 p2 = (vM[2] * vec4(vPos[2], 1.0f)).xyz;

	vec3 edge0 = p1 - p0;
	vec3 edge1 = p2 - p0;

	vec3 normal = cross(edge0, edge1);

	for (i = 0; i < 3; i++) {
		vec4 pos = vec4(vPos[i], 1.0f);

		/*
			|  X |  Z | MATH | Side      |
			|  0 | -1 |    0 | Forwards  |
			| -1 |  0 |    1 | Left      |
			|  1 |  0 |    2 | Right     |
			|  0 |  1 |    3 | Backwards |
			#+TBLFM: @2$3..@-0$3=round(($1+2*$2+1)/2)+1
		*/
		int side = int(normal.x + 2 * normal.y + 1) / 2 + 1;


		gTopY = vM[i] * vec4(pos.x, vTop[0], pos.z, 1);
		gBottomY = vM[i] * vec4(pos.x, vNeighbor[0][side], pos.z, 1);

		if (pos.y > 0)
			pos = gTopY;
		else
			pos = vM[i] * vec4(pos.x, -1000, pos.z, 1);//gBottomY;

		gPos = pos.xyz;

		mat3 normalMatrix = transpose(inverse(mat3(vM[i])));
		gNormal = normalize(normalMatrix * normal);

		gColor = vColor[i];
		gUV = vUV[i];
		gTBN = calcTBN(normalMatrix, normal, i);

		gl_Position = p * v * pos;
		EmitVertex();
	}
	EndPrimitive();
}
