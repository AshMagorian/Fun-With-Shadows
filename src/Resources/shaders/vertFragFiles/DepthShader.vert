#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 in_Model;
uniform mat4 in_View;
uniform mat4 in_Projection;

void main()
{
    gl_Position = in_Projection * in_View * in_Model * vec4(aPos, 1.0);
}