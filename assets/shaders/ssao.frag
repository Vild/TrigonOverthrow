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

void main()
{
  vec3 viewPosition = vec3(viewMatrix * texture(positionMap, uv));
  vec3 normal = vec3(texture(normalMap, uv));
  vec3 noise = vec3(texture(noiseMap, uv * noiseScale));

  vec3 tangent = normalize(noise - normal * dot(noise, normal));
  vec3 bitangent = cross(normal, tangent);
  mat3 TBN = mat3(tangent, bitangent, normal);

  occlusion = 0;

  for(int i = 0; i < sampleSize; i++)
  {
    vec3 samplePoint = TBN * samplePoints[i];
    samplePoint = viewPosition + samplePoint * sampleRadius;

    vec4 sampleProjection = projectionMatrix * vec4(samplePoint, 1);
    vec2 sampleUV = (sampleProjection.xy / sampleProjection.w) * 0.5 + 0.5;
    float sampleDepth = (viewMatrix * texture(positionMap, sampleUV)).z;

    float rangeCheck = smoothstep(0.0, 1.0, sampleRadius / abs(viewPosition.z - sampleDepth));
    occlusion += (sampleDepth >= samplePoint.z + sampleBias ? 1.0 : 0.0) * rangeCheck;
  }

  occlusion = 1.0 - (occlusion / sampleSize);
}
