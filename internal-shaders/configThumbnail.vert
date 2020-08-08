#version 430 core

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 texCoords;

out vec2 vTexCoords;

layout(std430, binding=4) buffer positionsBuffer{
   vec2 positions[];
};

void main(){
    vTexCoords = texCoords;
    gl_Position = vec4(positions[gl_InstanceID] + vertexPos, 0.0, 1.0);
}