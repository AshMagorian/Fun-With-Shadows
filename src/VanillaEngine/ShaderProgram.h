#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <vector>

class VertexArray;
class Texture;

struct Sampler
{
	GLint id;
	std::shared_ptr<Texture> texture;
};

class ShaderProgram : public Resource
{
private:
	GLuint id;
	std::vector<Sampler> samplers;

	void ReadFromFile(std::string _path, std::string& _text);
	void CompileShader(GLuint _id, std::string _shaderType);
	GLuint AttachVetexShader(std::string _path);
	GLuint AttachFragmentShader(std::string _path);

public:
	ShaderProgram();
	ShaderProgram(std::string _path);

	void Draw(std::shared_ptr<VertexArray> vertexArray);

	void SetUniform(std::string uniform, glm::vec2 value);
	void SetUniform(std::string uniform, glm::vec3 value);
	void SetUniform(std::string uniform, glm::vec4 value);
	void SetUniform(std::string uniform, float value);
	void SetUniform(std::string uniform, int value);
	void SetUniform(std::string uniform, glm::mat4 value);
	void SetUniform(std::string uniform, std::shared_ptr<Texture> texture);

	GLuint GetId() { return id; }
};
#endif