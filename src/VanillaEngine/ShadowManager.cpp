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

	//configure the depth map FBO
	glGenFramebuffers(1, &m_depthMapFBO);

	//configure the depth texture
	m_shadowWidth = 1024; m_shadowHeight = 1024;
	glGenTextures(1, &m_depthMapTex);
	glBindTexture(GL_TEXTURE_2D, m_depthMapTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowWidth, m_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMapTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowManager::RenderShadowMap()
{
	glViewport(0, 0, m_shadowWidth, m_shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
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

	glUseProgram(m_debugDepthShader->GetId());
	//m_debugDepthShader->SetUniform("near_plane", 1.0f);
	//m_debugDepthShader->SetUniform("far_plane", 7.5f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_depthMapTex);
	RenderQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void ShadowManager::RenderQuad()
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