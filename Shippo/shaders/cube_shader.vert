#version 460 core
layout (location = 0) in vec3 iPosition;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(iPosition, 1.0);
}