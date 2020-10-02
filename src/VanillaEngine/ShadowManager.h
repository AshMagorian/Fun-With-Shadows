#ifndef SHADOWMANAGER_H
#define SHADOWMANAGER_H

#include "Renderer.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

class Application;
class ShadowBox;
class ShadowShader;

class ShadowManager
{
	friend class Application;
private:


	std::weak_ptr<Application> m_application;
	std::shared_ptr<ShadowBox> m_shadowBox;
	std::shared_ptr<ShadowShader> m_shader;

	std::shared_ptr<ShaderProgram> m_debugDepthShader;
	void DebugDepthTexture();
	void RenderQuad();
	GLuint m_quadVAO = 0;
	GLuint m_quadVBO;

	std::list<ShadowData> m_objectData;

	int m_shadowWidth, m_shadowHeight;
	GLuint m_depthMapFBO;
	GLuint m_depthMapTex;
	glm::mat4 m_lightProjectionMatrix;

	void Init(std::weak_ptr<Application> _app);
	void RenderShadowMap();
public:
	~ShadowManager() { glDeleteFramebuffers(1, &m_depthMapFBO); }
	void addData(ShadowData _data) { m_objectData.push_back(_data); }

};
#endif