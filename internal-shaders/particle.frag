#version 430 core

in vec2 vTexCoords;
in vec3 vColor;

out vec4 out_color;

void main(void) {
  float d = length(vTexCoords - vec2(0.5));
  // float alpha = smoothstep(0.5, 0.4, d);
  if (d > 0.5)
    discard;
  vec3 col = vColor;
  out_color = vec4(col, 1.);
}