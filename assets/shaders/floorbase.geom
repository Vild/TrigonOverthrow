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

out vec3 gPos;
out vec3 gNormal;
out vec3 gColor;
out vec2 gUV;
out mat3 gTBN;

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

	vec3 triangleNormal = cross(edge0, edge1);
	if (setting_doBackFaceCulling)
		if (dot(normalize(p0 - cameraPos), triangleNormal) >= 0)
			return;

	for (i = 0; i < 3; i++) {
		vec4 pos = vec4(vPos[i], 1);
		pos.y = (pos.y > 0) ? vTop[0] : -100;
		pos = vM[i] * pos;
		gPos = pos.xyz;

		mat3 normalMatrix = transpose(inverse(mat3(vM[i])));
		gNormal = normalize(normalMatrix * triangleNormal);

		gColor = vColor[i];
		gUV = vUV[i];
		gTBN = calcTBN(normalMatrix, triangleNormal, i);

		gl_Position = p * v * pos;
		EmitVertex();
	}
	EndPrimitive();
}
