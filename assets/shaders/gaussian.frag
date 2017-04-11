#version 440 core
layout(location = 0) in vec2 frag_uv;
layout(location = 0) out vec3 frag_color;

uniform sampler2D image;

const int KERNEL_SIZE = 5;
const float weights[KERNEL_SIZE][KERNEL_SIZE] = {
  // {0.150342, 0.094907, 0.023792},
  // {0.094907, 0.059912, 0.015019},
  // {0.023792, 0.015019, 0.003765}
  {0.146634, 0.092566, 0.023205, 0.002289, 0.000088},
  {0.092566, 0.058434, 0.014648, 0.001445, 0.000055},
  {0.023205, 0.014648, 0.003672, 0.000362, 0.000014},
  {0.002289, 0.001445, 0.000362, 0.000036, 0.000001},
  {0.000088, 0.000055, 0.000014, 0.000001, 0.000000}
};

void main()
{
  vec2 texelSize = 1.0 / textureSize(image, 0);
  vec3 result = texture(image, frag_uv).rgb * weights[0][0];

  for (int x = 1; x < KERNEL_SIZE; x++)
  {
    vec2 xoffset = vec2(x * texelSize.x, 0);

    result += weights[0][x] * texture(image, frag_uv + xoffset).rgb;
    result += weights[0][x] * texture(image, frag_uv - xoffset).rgb;
  }

  for (int y = 1; y < KERNEL_SIZE; y++)
  {
    vec2 yoffset = vec2(0, y * texelSize.y);
    for (int x = 0; x < KERNEL_SIZE; x++)
    {
      vec2 xoffset = vec2(x * texelSize.x, 0);

      result += weights[y][x] * texture(image, frag_uv + xoffset + yoffset).rgb;
      result += weights[y][x] * texture(image, frag_uv - xoffset - yoffset).rgb;
    }
  }

  frag_color = result;
}
