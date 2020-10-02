#ifndef SHADOWSHADER_H
#define SHADOWSHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class VertexArray;

class ShadowShader
{
	friend class ShadowManager;
private:
	GLint id;

	void Init(std::string _vert, std::string _frag);

	GLuint AttachVetexShader(std::string _path);
	GLuint AttachFragmentShader(std::string _path);
	void ReadFromFile(std::string _path, std::string& _text);
	void CompileShader(GLuint _id, std::string _shaderType);

	void SetUniforms(glm::mat4 _view, glm::mat4 _projection);

	void Draw(std::list<ShadowData> _data);
};
#endif