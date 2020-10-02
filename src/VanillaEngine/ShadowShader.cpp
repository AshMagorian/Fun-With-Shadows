#include <VanillaEngine/VanillaEngine.h>

#include <fstream>

void ShadowShader::Init(std::string _vert, std::string _frag)
{
	id = glCreateProgram();
	
	GLuint vertexShaderId = AttachVetexShader(_vert);
	GLuint fragmentShaderId = AttachFragmentShader(_frag);
	glBindAttribLocation(id, 0, "in_Position");
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

GLuint ShadowShader::AttachVetexShader(std::string _path)
{
	std::string text;
	ReadFromFile(_path, text);
	const char *vertex = text.c_str();
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertex, NULL);
	CompileShader(vertexShaderId, "Vertex");
	return vertexShaderId;
}

GLuint ShadowShader::AttachFragmentShader(std::string _path)
{
	std::string text;
	ReadFromFile(_path, text);
	const char *fragment = text.c_str();
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragment, NULL);
	CompileShader(fragmentShaderId, "Fragment");
	return fragmentShaderId;
}

void ShadowShader::ReadFromFile(std::string _path, std::string& _text)
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

void ShadowShader::CompileShader(GLuint _id, std::string _shaderType)
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

void ShadowShader::SetUniforms(glm::mat4 _view, glm::mat4 _projection)
{
	glUseProgram(id);
	GLint uniformId = glGetUniformLocation(id, "in_View");
	if (uniformId == -1)
	{
		throw std::exception();
	}
	glUniformMatrix4fv(glGetUniformLocation(id, "in_View"), 1, GL_FALSE, glm::value_ptr(_view));
	glUniformMatrix4fv(glGetUniformLocation(id, "in_Projection"), 1, GL_FALSE, glm::value_ptr(_projection));
	glUseProgram(0);
}

void ShadowShader::Draw(std::list<ShadowData> _data)
{
	glUseProgram(id);

	for (std::list<ShadowData>::iterator i = _data.begin(); i != _data.end(); ++i)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, "in_Model"), 1, GL_FALSE, glm::value_ptr((*i).transform->GetModelMatrix()));

		glBindVertexArray((*i).VA->GetIdVertexOnly());
		glDrawArrays(GL_TRIANGLES, 0, (*i).VA->GetVertexCount());
	}
	glBindVertexArray(0);
	glUseProgram(0);
}