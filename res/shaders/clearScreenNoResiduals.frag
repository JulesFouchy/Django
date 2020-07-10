#version 430 core

uniform float alpha;
uniform vec3 backgroundColor;
uniform sampler2D prevFrame;
uniform float threshold;
uniform float minAlpha;

layout (location = 0) in vec2 uv;

void main(void){
    vec3 pCol = texture2D(prevFrame, uv).rgb;
    vec3 col = alpha * backgroundColor + (1.0 - alpha) * pCol;
    if (length( col - backgroundColor) < threshold) {
        float a = minAlpha;
        col = a * backgroundColor + (1.0 - a) * pCol;
        //col = backgroundColor;
    }
    gl_FragColor = vec4(col, 1.0);
}