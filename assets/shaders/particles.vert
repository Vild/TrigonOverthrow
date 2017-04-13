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

void main() {
	// Particle center is always in origin from the beginning.

	vUV = vec2(float(gl_InstanceID % 32 /32.0), float(gl_InstanceID / 32 / 32.0));
	vPos = texture(particlePos, vUV).xyz;
	//vPos = vec3((gl_InstanceID % 32) / 2.0, (gl_InstanceID / 32) /2.0, 0);
	vNormal = cameraPos - vPos;
	vColor = vec3(0, gl_InstanceID / 32 * 100, 0);

	gl_PointSize = 0.5f;
	gl_Position = p * v * vec4(vPos, 1.0f);
}