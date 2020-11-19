#version 430 core
layout (location = 0) in vec3 in_Pos;

out vec3 TexCoords;

uniform mat4 in_View;
uniform mat4 in_Projection;

void main()
{
	TexCoords = in_Pos;
	gl_Position = in_Projection * in_View * vec4(in_Pos, 1.0);
}