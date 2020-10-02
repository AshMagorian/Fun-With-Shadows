#include "VanillaEngine.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::OnInit(std::shared_ptr<ShaderProgram> _shader, std::shared_ptr<VertexArray> _va, std::shared_ptr<Texture> _tex)
{
	m_shaderProgram = _shader;
	m_va = _va;
	m_tex = _tex;
	m_cam = GetApplication()->GetCamera();
	m_shadowData.transform = GetEntity()->GetTransform();
	m_shadowData.VA = m_va;
	GetApplication()->GetShadowManager()->addData(m_shadowData);
}
void Renderer::OnTick()
{

}
void Renderer::OnDisplay()
{
	if (m_shaderProgram && m_tex && m_va)
	{
		m_shaderProgram->SetUniform("in_Projection", m_cam->GetProjectionMatrix());
		m_shaderProgram->SetUniform("in_View", m_cam->GetViewMatrix());
		m_shaderProgram->SetUniform("in_Model", GetEntity()->GetTransform()->GetModelMatrix());
		m_shaderProgram->SetUniform("in_Material.diffuse", m_tex);
		m_shaderProgram->SetUniform("in_Material.shininess", 32.0f);
		m_shaderProgram->Draw(m_va);
	}
}
