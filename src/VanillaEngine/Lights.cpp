#include "VanillaEngine.h"

Lights::Lights()
{
	m_directionalLight->ambient = glm::vec3(0.10f, 0.1f, 0.1f);
	m_directionalLight->diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
	m_directionalLight->specular = glm::vec3(0.5f, 0.2f, 0.5f);
	m_directionalLight->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
}

void Lights::UpdateLightShaderValues()
{
	for (std::list<std::shared_ptr<ShaderProgram>>::iterator i = m_shaderPrograms.begin(); i != m_shaderPrograms.end(); ++i)
	{
		(*i)->SetUniform("in_ViewPos", m_application.lock()->GetCamera()->GetCurrentCamera()->GetTransform()->GetPos());

		(*i)->SetUniform("in_DirLight.direction", m_directionalLight->direction);
		(*i)->SetUniform("in_DirLight.ambient", m_directionalLight->ambient);
		(*i)->SetUniform("in_DirLight.diffuse", m_directionalLight->diffuse);
		(*i)->SetUniform("in_DirLight.specular", m_directionalLight->specular);


		if (std::distance(m_pointLights.begin(), m_pointLights.end()) > 0)
		{
			for (std::list<std::shared_ptr<PointLight>>::iterator j = m_pointLights.begin(); j != m_pointLights.end(); ++j)
			{
				char buffer[64];
				int index = std::distance(m_pointLights.begin(), j);

				snprintf(buffer, sizeof(buffer), "in_PointLights[%i].position", index);
				(*i)->SetUniform(buffer, (*j)->m_position);

				snprintf(buffer, sizeof(buffer), "in_PointLights[%i].constant", index);
				(*i)->SetUniform(buffer, (*j)->m_constant);

				snprintf(buffer, sizeof(buffer), "in_PointLights[%i].linear", index);
				(*i)->SetUniform(buffer, (*j)->m_linear);

				snprintf(buffer, sizeof(buffer), "in_PointLights[%i].quadratic", index);
				(*i)->SetUniform(buffer, (*j)->m_quadratic);

				snprintf(buffer, sizeof(buffer), "in_PointLights[%i].ambient", index);
				(*i)->SetUniform(buffer, (*j)->m_ambient);

				snprintf(buffer, sizeof(buffer), "in_PointLights[%i].diffuse", index);
				(*i)->SetUniform(buffer, (*j)->m_diffuse);

				snprintf(buffer, sizeof(buffer), "in_PointLights[%i].specular", index);
				(*i)->SetUniform(buffer, (*j)->m_specular);
			}
		}
		if (std::distance(m_spotLights.begin(), m_spotLights.end()) > 0)
		{
			for (std::list<std::shared_ptr<SpotLight>>::iterator j = m_spotLights.begin(); j != m_spotLights.end(); ++j)
			{

				char buffer[64];
				int index = std::distance(m_spotLights.begin(), j);

				snprintf(buffer, sizeof(buffer), "in_SpotLights[%i].position", index);
				(*i)->SetUniform(buffer, (*j)->m_position);

				snprintf(buffer, sizeof(buffer), "in_SpotLights[%i].constant", index);
				(*i)->SetUniform(buffer, (*j)->m_constant);

				snprintf(buffer, sizeof(buffer), "in_SpotLights[%i].linear", index);
				(*i)->SetUniform(buffer, (*j)->m_linear);

				snprintf(buffer, sizeof(buffer), "in_SpotLights[%i].quadratic", index);
				(*i)->SetUniform(buffer, (*j)->m_quadratic);

				snprintf(buffer, sizeof(buffer), "in_SpotLights[%i].ambient", index);
				(*i)->SetUniform(buffer, (*j)->m_ambient);

				snprintf(buffer, sizeof(buffer), "in_SpotLights[%i].diffuse", index);
				(*i)->SetUniform(buffer, (*j)->m_diffuse);

				snprintf(buffer, sizeof(buffer), "in_SpotLights[%i].specular", index);
				(*i)->SetUniform(buffer, (*j)->m_specular);

				snprintf(buffer, sizeof(buffer), "in_SpotLights[%i].direction", index);
				(*i)->SetUniform(buffer, (*j)->m_direction);

				snprintf(buffer, sizeof(buffer), "in_SpotLights[%i].cutOff", index);
				(*i)->SetUniform(buffer, (*j)->m_cutOff);

				snprintf(buffer, sizeof(buffer), "in_SpotLights[%i].outerCutOff", index);
				(*i)->SetUniform(buffer, (*j)->m_outerCutOff);
			}
		}
	}
}

void Lights::UpdateShadowValues()
{
	for (std::list<std::shared_ptr<ShaderProgram>>::iterator i = m_shaderPrograms.begin(); i != m_shaderPrograms.end(); ++i)
	{
		(*i)->SetUniform("in_LightSpaceMatrix", m_application.lock()->GetShadowManager()->GetLightSpaceMatrix());
		(*i)->SetUniform("in_ShadowMap", m_application.lock()->GetShadowManager()->GetShadowMap());
	}
}

void Lights::AddPointLight(std::shared_ptr<Entity> _entity)
{
	try
	{
		if ((int)m_pointLights.size() > 49) { throw Exception("Point light not created, Can't have more than 50 point lights"); }
		std::shared_ptr<PointLight> rtn = _entity->GetComponent<PointLight>();
		m_pointLights.push_back(rtn);
		for (std::list<std::shared_ptr<ShaderProgram>>::iterator i = m_shaderPrograms.begin(); i != m_shaderPrograms.end(); ++i)
		{
			(*i)->SetUniform("in_NoPointLights", (int)m_pointLights.size());
		}
	}
	catch (Exception& e)
	{
		std::cout << "VanillaEngine Exception: " << e.what() << std::endl;
	}
}

void Lights::AddSpotLight(std::shared_ptr<Entity> _entity)
{
	try
	{
		if ((int)m_spotLights.size() > 49) { throw Exception("Spotlight not created, Can't have more than 50 spotlights"); }
		std::shared_ptr<SpotLight> rtn = _entity->GetComponent<SpotLight>();
		m_spotLights.push_back(rtn);
		for (std::list<std::shared_ptr<ShaderProgram>>::iterator i = m_shaderPrograms.begin(); i != m_shaderPrograms.end(); ++i)
		{
			(*i)->SetUniform("in_NoSpotLights", (int)m_spotLights.size());
		}
	}
	catch (Exception& e)
	{
		std::cout << "VanillaEngine Exception: " << e.what() << std::endl;
	}
}