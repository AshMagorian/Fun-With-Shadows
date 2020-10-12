#version 430 core

layout (location = 0) in vec3 in_Position;

out VS_OUT {
	vec4 Color;
} vs_out;

uniform vec4 in_Color;

uniform mat4 in_View;
uniform mat4 in_Projection;
uniform mat4 in_Model;

void main()
{
	gl_Position = in_Projection * in_View * in_Model * vec4(in_Position, 1.0);
	vs_out.Color = in_Color;
}