#version 440 core

out vec4 outColor;

in vec3 vColor;
in vec2 vUV;

uniform sampler2D defPos;
uniform sampler2D defNormal;
uniform sampler2D defDiffuseSpecular;
uniform sampler2D defDepth;
uniform sampler2D defOcclusionMap;

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 cameraPos;

uniform DirLight setting_dirLight;
uniform float settings_shininess = 1f;

void main() {
	vec3 pos = texture(defPos, vUV).xyz;
	vec3 normal = texture(defNormal, vUV).xyz;
	vec3 diffuse = texture(defDiffuseSpecular, vUV).xyz;
	float specular = texture(defDiffuseSpecular, vUV).w;
	float depth = texture(defDepth, vUV).r;
	float occlusion = texture(defOcclusionMap, vUV).r;

	vec3 viewDir = normalize(cameraPos - pos);
	vec3 lightDir = normalize(-setting_dirLight.direction);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), settings_shininess);
	// Combine results
	vec3 a = setting_dirLight.ambient * diffuse;
	vec3 d = setting_dirLight.diffuse * diff * diffuse;
	vec3 s = vec3(0); //setting_dirLight.specular * spec * specular;

	outColor = vec4(normal, 1);//vec4(a + d + s, 1);// * occlusion;
	gl_FragDepth = depth;
}
