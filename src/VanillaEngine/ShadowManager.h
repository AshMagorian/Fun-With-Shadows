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

	bool m_first = true;
	std::weak_ptr<Application> m_application;
	std::shared_ptr<ShadowBox> m_shadowBox;
	std::shared_ptr<ShadowShader> m_shader;

	std::shared_ptr<ShaderProgram> m_debugDepthShader;
	void DebugDepthTexture();

	std::list<ShadowData> m_objectData;

	std::shared_ptr<RenderTexture> m_depthMap;

	int m_shadowWidth, m_shadowHeight;
	glm::mat4 m_lightViewMatrix;
	glm::mat4 m_lightProjectionMatrix;

	void Init(std::weak_ptr<Application> _app);
	void CalcInitialLightViewMatrix();
	void RenderShadowMap();
	void CalcViewMatrix(glm::vec3 _lightDir, glm::vec3 _center);
	void CalcOrthoProjectionMatrix(float _w, float _h, float _l);
public:
	void AddData(ShadowData _data);
	void RemoveData(ShadowData _data);

	glm::mat4 GetLightViewMatrix() { return m_lightViewMatrix; }
	glm::mat4 GetLightSpaceMatrix() { return (m_lightProjectionMatrix * m_lightViewMatrix); }
	std::shared_ptr<Texture> GetShadowMap() { return std::static_pointer_cast<Texture>(m_depthMap); }
};
#endif