#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class ShaderProgram;
class Application;

struct BoxTexture
{
	GLuint id;
	std::string name;
};

class Skybox
{
private:
	GLuint textureID;
	GLuint vaID;
	std::vector<BoxTexture> m_boxTextures;
	std::vector<std::string> m_texturesFaces;
	std::shared_ptr<ShaderProgram> m_shader;
	std::weak_ptr<Application> m_application;

	void InitBoxVertexArray();
	void SetFaces(std::string _rt, std::string _lt, std::string _tp, std::string _bm, std::string _bk, std::string _ft);
	void SetShaderUniforms();

public:
	Skybox() {}

	void Init(std::weak_ptr<Application>_app);

	/*
	*Creates a BoxTexture and pushes it back onto the m_BoxTextures vector
	*/
	void CreateSkybox(std::string _name, std::string _right, std::string _left, std::string _top, std::string _bottom, std::string _back, std::string _front);
	
	/*
	*Sets the named skybox as the current one
	*/
	void SetSkybox(std::string _name);

	void DrawSkybox();
};
#endif