#version 430 core

uniform float alpha;
uniform vec3 backgroundColor;
uniform sampler2D prevFrame;
uniform float threshold;
uniform float minAlpha;

layout(location = 0) in vec2 uv;

out vec4 out_color;

void main(void) {
  vec3 prevCol = texture2D(prevFrame, uv).rgb;
  float a = length(prevCol - backgroundColor) < threshold ? minAlpha : alpha;
  out_color = vec4(a * backgroundColor + (1.0 - a) * prevCol, 1.0);
}