#version 430 core

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 texCoords;
//layout (location = 2) in vec2 partPos;

out vec2 vTexCoords;

layout(std430, binding = 2) buffer _partPos
{
    vec2 partPos[];
};

void main(void){
    vTexCoords = texCoords;
    gl_Position = vec4(vertexPos+partPos[gl_InstanceID], 0.0, 1.0);
}