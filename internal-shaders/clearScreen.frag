#version 430 core

uniform float alpha;
uniform vec3 backgroundColor;

void main(void){
    gl_FragColor = vec4(backgroundColor, alpha);
}