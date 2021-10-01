#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * aPos;
	texCoord = aTex;
}