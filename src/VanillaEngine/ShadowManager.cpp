#include <VanillaEngine/VanillaEngine.h>

void ShadowManager::Init(std::weak_ptr<Application> _app) 
{ 
	m_application = _app; 
	m_shadowBox = std::make_shared<ShadowBox>();
	m_shadowBox->SetCamera(m_application.lock()->GetCamera());
	m_shadowBox->CalcFrustrumWidthsAndHeights();
	m_shader = std::make_shared<ShadowShader>();
	try
	{
		m_shader->Init("../src/resources/shaders/vertFragFiles/DepthShader.vert", "../src/resources/shaders/vertFragFiles/DepthShader.frag");
	}
	catch (Exception& e)
	{
		std::cout << "VanillaEngine Exception: " << e.what() << std::endl;
	}
	m_debugDepthShader = std::make_shared<ShaderProgram>("../src/resources/shaders/debugDepthShader.txt");

	m_shadowWidth = 1024; m_shadowHeight = 1024;

	m_depthMap = std::make_shared<RenderTexture>(m_shadowWidth, m_shadowHeight);
	m_depthMap->InitDepthOnly();
	m_depthMap->SetFilteringNearest();
	m_depthMap->SetWrapClampBorder(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void ShadowManager::RenderShadowMap()
{
	glViewport(0, 0, m_shadowWidth, m_shadowHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap->GetFboId());

	glClear(GL_DEPTH_BUFFER_BIT);

	m_lightProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 8.0f);

	m_shadowBox->Update(); 
	m_shader->SetUniforms(m_shadowBox->m_lightViewMatrix, m_lightProjectionMatrix);
	//m_shader->SetUniforms(m_application.lock()->GetCamera()->GetViewMatrix(), m_application.lock()->GetCamera()->GetProjectionMatrix());
	m_shader->Draw(m_objectData);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowManager::DebugDepthTexture()
{
	glViewport(0, 0, m_shadowWidth / 3, m_shadowHeight / 3);

	//m_debugDepthShader->SetUniform("near_plane", 1.0f);
	//m_debugDepthShader->SetUniform("far_plane", 7.5f);

	std::shared_ptr<Texture> tex = std::dynamic_pointer_cast<Texture>(m_depthMap);
	m_debugDepthShader->Draw(tex);
}