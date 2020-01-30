#version 430 core

in vec2 vTexCoords;

void main(void){
    float d = length(vTexCoords - vec2(0.5));
    float alpha = smoothstep(0.5, 0.4, d);
    float t = smoothstep(0.5, 0.15, d);
    vec3 col = t * vec3(0.8, 0.5, 0.6);
    gl_FragColor = vec4(col, alpha);
}