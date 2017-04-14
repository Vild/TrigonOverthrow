#version 440 core
layout(location = 0) in vec2 uv;
layout(location = 0) out float occlusion;

uniform sampler2D positionMap;
uniform sampler2D normalMap;
uniform sampler2D noiseMap;

uniform vec2 noiseScale;

uniform int sampleSize;
uniform float sampleRadius;
uniform float sampleBias;
uniform vec3 samplePoints[64];

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

float getDepth(vec3 position)
{
  vec4 projected = projectionMatrix * vec4(position, 1);
  vec2 pUV = (projected.xy / projected.w) * 0.5 + 0.5;
  return (viewMatrix * vec4(texture(positionMap, pUV).rgb, 1)).z;
}

void main()
{
  vec3 viewPosition = vec3(viewMatrix * texture(positionMap, uv));
  vec3 normal = vec3(viewMatrix * vec4(texture(normalMap, uv).rgb, 0));
  vec3 noise = vec3(texture(noiseMap, uv * noiseScale));

  vec3 tangent = normalize(noise - normal * dot(noise, normal));
  vec3 bitangent = cross(normal, tangent);
  mat3 TBN = mat3(tangent, bitangent, normal);

  occlusion = 0.0;

  for(int i = 0; i < sampleSize; i++)
  {
    vec3 samplePoint = viewPosition + samplePoints[i] * sampleRadius;
    float sampleDepth = getDepth(samplePoint);

    float rangeCheck = smoothstep(0.0, 1.0, sampleRadius / abs(viewPosition.z - sampleDepth));
    occlusion += (sampleDepth >= samplePoint.z + sampleBias ? 1.0 : 0.0) * rangeCheck;
  }

  occlusion = 1.0 - (occlusion / sampleSize);
}
