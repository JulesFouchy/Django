#version 430 core

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 texCoords;

out vec2 vTexCoords;
out vec3 vColor;

struct particle{
    vec2 pos;
    vec2 vel;
};

layout(std430, binding=0) buffer particles{
   particle part[];
};
layout(std430, binding=2) buffer colors{
   float channel[];
};

void main(){
    vTexCoords = texCoords;
    vColor = vec3(channel[3*gl_InstanceID],channel[3*gl_InstanceID+1],channel[3*gl_InstanceID+2]);
    gl_Position = vec4(vertexPos+part[gl_InstanceID].pos, 0.0, 1.0);
}