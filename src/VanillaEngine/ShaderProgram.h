#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <vector>

#include "Resource.h"

class RenderTexture;
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

	void CreateShaderProgram(std::string _vert, std::string _frag);
	void ReadFromFile(std::string _path, std::string& _text);
	void CompileShader(GLuint _id, std::string _shaderType);
	GLuint AttachVetexShader(std::string _path);
	GLuint AttachFragmentShader(std::string _path);

	GLuint m_quadVAO = 0;
	GLuint m_quadVBO;

public:
	ShaderProgram(std::string _path);
	ShaderProgram(std::string _vert, std::string _frag);

	void Draw(std::shared_ptr<VertexArray> vertexArray);
	void Draw(std::shared_ptr<RenderTexture> renderTexture);
	void Draw(std::shared_ptr<Texture> texture);
	void RenderQuad();

	void SetUniform(std::string uniform, glm::vec2 value);
	void SetUniform(std::string uniform, glm::vec3 value);
	void SetUniform(std::string uniform, glm::vec4 value);
	void SetUniform(std::string uniform, float value);
	void SetUniform(std::string uniform, int value);
	void SetUniform(std::string uniform, glm::mat4 value);
	void SetUniform(std::string uniform, std::shared_ptr<Texture> texture);

	std::vector<Sampler> GetSamplers() { return samplers; }
	GLuint GetId() { return id; }
};
#endif