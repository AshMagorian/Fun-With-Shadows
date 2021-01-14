#version 430 core

layout (location = 0) in vec3 in_Position;
layout (location = 2) in vec2 in_TexCoord;
layout (location = 3) in vec3 in_Normal;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
	float Visibility;
} vs_out;

const float density = 0.025;
const float gradient = 3.0;

uniform mat4 in_View;
uniform mat4 in_Projection;
uniform mat4 in_Model;
uniform mat4 in_LightSpaceMatrix;

void main()
{
	vs_out.FragPos = vec3(in_Model * vec4(in_Position, 1.0));
	vec4 relativeCamPosition = in_View * in_Model * vec4(in_Position, 1.0);
	vs_out.TexCoord = in_TexCoord;
	vs_out.Normal = mat3(in_Model) * in_Normal;
	vs_out.FragPosLightSpace = in_LightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
	gl_Position = in_Projection * relativeCamPosition;
	
	float distance = length(relativeCamPosition.xyz);
	vs_out.Visibility = exp(-pow((distance*density), gradient));
	vs_out.Visibility = clamp(vs_out.Visibility, 0.0, 1.0);
}