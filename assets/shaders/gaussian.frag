#version 440 core
layout(location = 0) in vec2 uv;
layout(location = 0) out vec3 color;

uniform sampler2D image;

uniform bool horizontal;

uniform int kernelSize;
uniform float kernel[64];

void main()
{
  vec2 texelSize = 1.0 / textureSize(image, 0);
  vec3 result = texture(image, uv).rgb * kernel[0];

  vec2 xoffset = vec2(texelSize.x, 0);
  vec2 yoffset = vec2(0, texelSize.y);

  if (horizontal) {
    for (int x = 1; x < kernelSize; x++) {
      result += kernel[x] * texture(image, uv + x * xoffset).rgb;
      result += kernel[x] * texture(image, uv - x * xoffset).rgb;
    }
  } else {
    for (int y = 1; y < kernelSize; y++) {
      result += kernel[y] * texture(image, uv + y * yoffset).rgb;
      result += kernel[y] * texture(image, uv - y * yoffset).rgb;
    }
  }

  color = result;
}
