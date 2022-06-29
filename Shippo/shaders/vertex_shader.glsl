#version 460 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;

uniform mat4 mvp;
uniform mat4 model;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

void main()
{
    gl_Position = mvp * vec4(iPosition, 1.0);
    fragPos = vec3(model * vec4(iPosition, 1.0));
    normal = mat3(transpose(inverse(model))) * iNormal;
    texCoord = iTexCoord;
}