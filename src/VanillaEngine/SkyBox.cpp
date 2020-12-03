#include <VanillaEngine/VanillaEngine.h>
#include "stb_image.h"
#include <iostream>

void Skybox::Init(std::weak_ptr<Application>_app)
{
	m_application = _app;
	m_texturesFaces.resize(6);
	InitBoxVertexArray();
	try
	{
		m_shader = std::make_shared<ShaderProgram>("../src/resources/shaders/skybox.vert", "../src/resources/shaders/skybox.frag");
		glUseProgram(m_shader->GetId());
		glUniform1i(glGetUniformLocation(m_shader->GetId(), "skybox"), 0);
		glUseProgram(0);
	}
	catch (Exception& e)
	{
		std::cout << "VanillaEngine Exception: " << e.what() << std::endl;
	}
}

void Skybox::InitBoxVertexArray()
{
	std::vector<float> skyboxVertices = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &vaID);
	glGenBuffers(1, &vbID);
	glBindVertexArray(vaID);

	glBindBuffer(GL_ARRAY_BUFFER, vbID);
	glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), &skyboxVertices.at(0), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GLfloat), (void *)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Skybox::SetFaces(std::string _rt, std::string _lt, std::string _tp, std::string _bm, std::string _bk, std::string _ft)
{
	m_texturesFaces[0] = _rt;
	m_texturesFaces[1] = _lt;
	m_texturesFaces[2] = _tp;
	m_texturesFaces[3] = _bm;
	m_texturesFaces[4] = _bk;
	m_texturesFaces[5] = _ft;
}

void Skybox::CreateSkybox(std::string _name, std::string _right, std::string _left, std::string _top, std::string _bottom, std::string _back, std::string _front)
{

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	unsigned char *data;
	SetFaces(_right, _left, _top, _bottom, _back, _front);
	for (unsigned int i = 0; i < m_texturesFaces.size(); i++)
	{
		data = stbi_load(m_texturesFaces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			//stbi_image_free(data);
			free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << m_texturesFaces[i].c_str() << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

//void Skybox::SetSkybox(std::string _name)
//{
//	for (int i = 0; i < m_boxTextures.size(); ++i)
//	{
//		if (_name == m_boxTextures[i].name)
//		{
//			textureID = m_boxTextures[i].id;
//		}
//	}
//}

void Skybox::DrawSkybox()
{
	glDepthMask(GL_FALSE);

	SetShaderUniforms();

	glUseProgram(m_shader->GetId());

	glBindVertexArray(vaID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthMask(GL_TRUE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	
}

void Skybox::SetShaderUniforms()
{
	glm::mat4 view = glm::mat4(glm::mat3(m_application.lock()->GetCamera()->GetViewMatrix()));
	glm::mat4 proj = m_application.lock()->GetCamera()->GetProjectionMatrix();

	m_shader->SetUniform("in_View", view);
	m_shader->SetUniform("in_Projection", proj);
}