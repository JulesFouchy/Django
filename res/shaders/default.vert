#version 430 core

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 texCoords;

out vec2 vTexCoords;

layout(std430, binding = 0) buffer actualPositions
{
    vec2 partPos[];
};

void main(){
    vTexCoords = texCoords;
    gl_Position = vec4(vertexPos+partPos[gl_InstanceID], 0.0, 1.0);
}