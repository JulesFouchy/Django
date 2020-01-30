#version 430 core

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 texCoords;

out vec2 vTexCoords;

struct particle{
    vec2 pos;
    vec2 vel;
};

layout(std430, binding=0) buffer particles{
   particle part[];
};

void main(){
    vTexCoords = texCoords;
    gl_Position = vec4(vertexPos+part[gl_InstanceID].pos, 0.0, 1.0);
}