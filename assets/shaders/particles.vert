#version 440 core
// NOTE: The locations must match ShaderAttributeID
out vec3 vPos;
out vec3 vNormal;
out vec3 vColor;
out vec2 vUV;

uniform vec3 cameraPos;
uniform mat4 v;
uniform mat4 p;
uniform sampler2D particlePos;
uniform sampler2D particleVel;
uniform int textureSize;

void main() {
	// Particle center is always in origin from the beginning.

	vUV = vec2(float(gl_InstanceID % textureSize / float(textureSize)), float(gl_InstanceID / textureSize / float(textureSize)));
	vPos = texture(particlePos, vUV).xyz;
	//vPos = vec3((gl_InstanceID % 32) / 2.0, (gl_InstanceID / 32) /2.0, 0);
	vNormal = cameraPos - vPos;
	vColor = texture(particleVel, vUV).xyz;
	vColor = vec3(sin(vColor.x), cos(vColor.y), sin(vColor.z));
	gl_PointSize = 2;
	gl_Position = p * v * vec4(vPos, 1.0f);
}