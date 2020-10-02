#include "VanillaEngine.h"

#include <glm/ext.hpp>
#include <iostream>

ShaderProgram::ShaderProgram()
{
	id = glCreateProgram();
}

/**
*The file passed through contains the paths for both shader files. This is because to make
*Shaderprogram a resource it needed to take only one paramater as opposed to two
*/
ShaderProgram::ShaderProgram(std::string _path)
{
	std::string vertPath;
	std::string fragPath;

	std::ifstream file(_path);

	if (!file.is_open())
	{
		throw Exception("Shader file not found, '" + _path + "' cannot be loaded");
	}
	else
	{
		std::string line;
		std::getline(file, line);
		vertPath += line;
		std::getline(file, line);
		fragPath += line;
	}
	file.close();

	id = glCreateProgram();

	GLuint vertexShaderId = AttachVetexShader(vertPath);
	GLuint fragmentShaderId = AttachFragmentShader(fragPath);

	glBindAttribLocation(id, 0, "in_Position");
	glBindAttribLocation(id, 1, "in_Color");
	glBindAttribLocation(id, 2, "in_TexCoord");
	glBindAttribLocation(id, 3, "in_Normal");

	// Perform the link and check for faliure
	glLinkProgram(id);
	GLint success = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		throw Exception("Shader cannot be created");
	}
	glUseProgram(id);

	glDetachShader(id, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(id, fragmentShaderId);
	glDeleteShader(fragmentShaderId);
}

void ShaderProgram::ReadFromFile(std::string _path, std::string& _text)
{
	std::ifstream file(_path);
	if (!file.is_open())
	{
		throw Exception("Vertex shader not found, '" + _path + "' cannot be loaded");
	}
	else
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			_text += line + "\n";
		}
	}
	file.close();
}

void ShaderProgram::CompileShader(GLuint _id, std::string _shaderType)
{
	glCompileShader(_id);
	GLint success = 0;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint maxLength = 0;
		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorlog(maxLength);
		glGetShaderInfoLog(_id, maxLength, &maxLength, &errorlog[0]);
		throw Exception(_shaderType + " shader compile error: " + (std::string)&errorlog.at(0));
	}
	glAttachShader(id, _id);
}

GLuint ShaderProgram::AttachVetexShader(std::string _path)
{
	std::string text;
	ReadFromFile(_path, text);
	const char *vertex = text.c_str();
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertex, NULL);
	CompileShader(vertexShaderId, "Vertex");
	return vertexShaderId;
}

GLuint ShaderProgram::AttachFragmentShader(std::string _path)
{
	std::string text;
	ReadFromFile(_path, text);
	const char *fragment = text.c_str();
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragment, NULL);
	CompileShader(fragmentShaderId, "Fragment");
	return fragmentShaderId;
}

void ShaderProgram::Draw(std::shared_ptr<VertexArray> vertexArray)
{
	// Instruct OpenGL to use our shader program and our VAO
	glUseProgram(id);
	glBindVertexArray(vertexArray->GetId());

	for (size_t i = 0; i < samplers.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		if (samplers.at(i).texture)
		{
			glBindTexture(GL_TEXTURE_2D, samplers.at(i).texture->getId());
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetVertexCount());
	//Unbinds the textures for all samplers
	for (size_t i = 0; i < samplers.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Reset the state
	glBindVertexArray(0);
	glUseProgram(0);
}

void ShaderProgram::Draw(std::shared_ptr<RenderTexture> renderTexture)
{

}

void ShaderProgram::Draw(std::shared_ptr<Texture> texture)
{
	glUseProgram(id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getId());
	RenderQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void ShaderProgram::RenderQuad()
{
	if (m_quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &m_quadVAO);
		glGenBuffers(1, &m_quadVBO);
		glBindVertexArray(m_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(m_quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void ShaderProgram::SetUniform(std::string uniform, glm::vec2 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform2f(uniformId, value.x, value.y);
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, glm::vec3 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform3f(uniformId, value.x, value.y, value.z);
	glUseProgram(0);
}
void ShaderProgram::SetUniform(std::string uniform, glm::vec4 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform4f(uniformId, value.x, value.y, value.z, value.w);
	glUseProgram(0);
}
void ShaderProgram::SetUniform(std::string uniform, float value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform1f(uniformId, value);
	glUseProgram(0);
}
void ShaderProgram::SetUniform(std::string uniform, int value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform1i(uniformId, value);
	glUseProgram(0);
}
void ShaderProgram::SetUniform(std::string uniform, glm::mat4 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(value));
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, std::shared_ptr<Texture> texture)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}
	//Tries to find the correct sampler in the vector
	for (size_t i = 0; i < samplers.size(); i++)
	{
		if (samplers.at(i).id == uniformId)
		{
			samplers.at(i).texture = texture;

			glUseProgram(id);
			glUniform1i(uniformId, i);
			glUseProgram(0);
			return;
		}
	}
	//If the sampler isn't found then a new sampler is made and pushed back
	Sampler s;
	s.id = uniformId;
	s.texture = texture;
	samplers.push_back(s);

	glUseProgram(id);
	glUniform1i(uniformId, samplers.size() - 1);
	glUseProgram(0);
}