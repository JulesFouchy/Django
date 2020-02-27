#version 430 core

uniform float dt;
uniform float decay;

void main(void){
    gl_FragColor = vec4(0., 0., 0., decay * dt);
}