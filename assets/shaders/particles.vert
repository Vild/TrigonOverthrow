#version 440 core
// NOTE: The locations must match ShaderAttributeID
out vec3 vPos;
out vec3 vNormal;
out vec3 vColor;
out vec2 vUV;

uniform mat4 v;
uniform mat4 p;
uniform sampler2D particlePos;
uniform sampler2D particleVel;
uniform int textureSize;
uniform int emitterCount;

void main() {
	// Particle center is always in origin from the beginning.

	vUV = vec2(float(gl_InstanceID % textureSize / float(textureSize)), float(gl_InstanceID / textureSize / float(textureSize)));
	vPos = texture(particlePos, vUV).xyz;
	//vPos = vec3((gl_InstanceID % 32) / 2.0, (gl_InstanceID / 32) /2.0, 0);
	//vColor = texture(particleVel, vUV).xyz;
	vColor = vec3(clamp(sin(vColor.x), 0.01, 1.0), clamp(cos(vColor.y), 0.1, 1.0), sin(vColor.z));
	//vColor = vec3(0,1,0);
	gl_PointSize = 2;
	gl_Position = p * v * vec4(vPos, 1.0f);
}