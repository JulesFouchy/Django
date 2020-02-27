#version 430 core

in vec2 vTexCoords;
in vec3 vColor;

void main(void){
    float d = length(vTexCoords - vec2(0.5));
    float alpha = smoothstep(0.5, 0.4, d);
    float t = smoothstep(0.5, 0.15, d);
    //vec3 col = t * vColor;
    vec3 col = vColor;
    gl_FragColor = vec4(col, alpha);
}