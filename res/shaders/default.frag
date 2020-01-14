#version 430 core

in vec2 vTexCoords;

void main(void){
    float d = length(vTexCoords - vec2(0.5));
    float alpha = smoothstep(0.5, 0.35, d);
    gl_FragColor = vec4(0.8, 0.7, 0.9, alpha);
}