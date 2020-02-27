#version 430 core

uniform float dt;
uniform float decay;
uniform vec3 backgroundColor;

void main(void){
    gl_FragColor = vec4(backgroundColor, decay * dt);
}