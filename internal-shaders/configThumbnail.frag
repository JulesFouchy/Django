#version 430 core

in vec2 vTexCoords;

void main(void){
    float d = length(vTexCoords - vec2(0.5));
    if (d > 0.5)
        discard;
    gl_FragColor = vec4(1., 1., 1., 1.);
}