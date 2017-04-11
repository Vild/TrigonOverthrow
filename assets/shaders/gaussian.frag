#version 440 core
layout(location = 0) out vec3 frag_color;
layout(location = 0) in vec2 frag_uv;

uniform sampler2D image;

const int KERNEL_SIZE = 3;
const float weights[KERNEL_SIZE][KERNEL_SIZE] = {
  {0.150342, 0.094907, 0.023792},
  {0.094907, 0.059912, 0.015019},
  {0.023792, 0.015019, 0.003765}
};

void main()
{
  vec2 texelSize = 1.0 / textureSize(image, 0);
  vec3 result = texture(image, frag_uv).rgb * weights[0][0];

  for (int x = 1; x < KERNEL_SIZE; y++)
  {
    vec2 xoffset = vec2(x * texelSize.x, 0);
    for (int y = 1; y < KERNEL_SIZE; y++)
    {
      vec2 yoffset = vec2(0, y * texelSize.y);

      result += weights[i][0] * texture(image, frag_uv + xoffset).rgb;
      result += weights[i][0] * texture(image, frag_uv - xoffset).rgb;
    }
  }

}
