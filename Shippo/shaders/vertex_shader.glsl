#version 460 core

in vec3 position;
in vec2 vTexCoord;

uniform mat4 mvp;

out vec2 texCoord;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    texCoord = vTexCoord;
}