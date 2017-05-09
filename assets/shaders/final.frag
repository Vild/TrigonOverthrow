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
	vec3 diffuse;
	vec3 specular;

	vec3 direction;
};

struct PointLight {
	vec3 diffuse;
	vec3 specular;

	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

uniform bool settings_enableDirLight;
uniform bool settings_enablePointLight;
uniform float settings_shininess;

uniform vec3 cameraPos;

uniform vec3 ambient;
#define MAX_POINT_LIGHTS 64
uniform DirLight dirLight;
uniform int pointLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

vec3 calcDirLight(DirLight light, vec3 pos, vec3 normal, vec3 diffuse, float specular) {
	// Diffuse shading
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	vec3 viewDir = normalize(cameraPos - pos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), settings_shininess);

	// Combine results
	vec3 d = light.diffuse * diff * diffuse;
	vec3 s = light.specular * spec * specular;
	return d + s;
}

vec3 calcPointLight(PointLight pointLight, vec3 pos, vec3 normal, vec3 diffuse, float specular) {
	float distance = length(pointLight.position - pos);
	float attenuation = 1.0f / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

	// Diffuse shading
	vec3 lightDir = normalize(pointLight.position - pos);
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	vec3 viewDir = normalize(cameraPos - pos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), settings_shininess);

	// Combine results
	vec3 d = pointLight.diffuse * diff * diffuse;
	vec3 s = pointLight.specular * spec * specular;
	return (d + s) * attenuation;
}

void main() {
	vec3 pos = texture(defPos, vUV).xyz;
	vec3 normal = texture(defNormal, vUV).xyz;
	vec3 diffuse = texture(defDiffuseSpecular, vUV).xyz;
	float specular = texture(defDiffuseSpecular, vUV).w;
	float depth = texture(defDepth, vUV).r;
	float occlusion = texture(defOcclusionMap, vUV).r;

	vec3 result = vec3(0);

	if (settings_enableDirLight)
		result += ambient * diffuse + calcDirLight(dirLight, pos, normal, diffuse, specular);

	if (settings_enablePointLight)
		for (int i = 0; i < pointLightCount; i++)
			result += calcPointLight(pointLights[i], pos, normal, diffuse, specular);

	if (!settings_enableDirLight && !settings_enablePointLight)
		result = diffuse;

	result *= occlusion;
	outColor = vec4(result, 1);
	gl_FragDepth = depth;
}
