#version 430 core

uniform float dt;

void main(void){
    gl_FragColor = vec4(0., 0., 0., 20.0 * dt);
}