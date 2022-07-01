#version 430 core

in vec2 vTexCoords;

out vec4 out_color;

void main(void) {
  float d = length(vTexCoords - vec2(0.5));
  if (d > 0.5)
    discard;
  out_color = vec4(1., 1., 1., 1.);
}