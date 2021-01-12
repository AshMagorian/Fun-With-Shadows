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
	GetApplication()->GetShadowManager()->AddData(m_shadowData);

}
void Renderer::OnInit(std::shared_ptr<ShaderProgram> _shader, std::shared_ptr<VertexArray> _va, glm::vec4 _color)
{
	m_shaderProgram = _shader;
	m_va = _va;
	m_color = _color;
	m_cam = GetApplication()->GetCamera();
	m_shadowData.transform = GetEntity()->GetTransform();
	m_shadowData.VA = m_va;
	GetApplication()->GetShadowManager()->AddData(m_shadowData);

}

void Renderer::InitShadowData()
{

}

void Renderer::OnTick()
{

}
void Renderer::OnDisplay()
{
	if (m_shaderProgram && m_va)
	{
		m_shaderProgram->SetUniform("in_Projection", m_cam->GetProjectionMatrix());
		m_shaderProgram->SetUniform("in_View", m_cam->GetViewMatrix());
		m_shaderProgram->SetUniform("in_Model", GetEntity()->GetTransform()->GetModelMatrix());
		if (m_tex)
		{
			m_shaderProgram->SetUniform("in_Material.diffuse", m_tex);
			m_shaderProgram->SetUniform("in_Material.shininess", 4.0f);
		}
		else
		{
			m_shaderProgram->SetUniform("in_Color", m_color);
		}
		m_shaderProgram->Draw(m_va);
	}
}

void Renderer::ToggleShadow(bool _value)
{
	if (_value == true)
	{
		GetApplication()->GetShadowManager()->AddData(m_shadowData);
	}
	else
	{
		GetApplication()->GetShadowManager()->RemoveData(m_shadowData);
	}
}