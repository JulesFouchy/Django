#version 430 core

uniform float alpha;
uniform vec3 backgroundColor;

out vec4 out_color;

void main(void) { out_color = vec4(backgroundColor, alpha); }