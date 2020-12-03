#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
	vec3 tex = TexCoords;

	if (tex.x > 0.99999) {tex.z *= -1.0;}
	if (tex.y > 0.99999) {tex.z *= -1.0;}
	if (tex.z > 0.99999) {tex.x *= -1.0;}
	if (tex.x < -0.99999) {tex.z *= -1.0;}
	if (tex.y < -0.99999) {tex.z *= -1.0;}
	if (tex.z < -0.99999) {tex.x *= -1.0;}

	FragColor = texture(skybox, tex);
}