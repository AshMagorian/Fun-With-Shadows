#include <VanillaEngine/VanillaEngine.h>

#include <iostream>

TerrainRenderer::TerrainRenderer()
{

}

void TerrainRenderer::Init(std::weak_ptr<Application> _app)
{
	m_application = _app;

	try
	{
		m_shader = std::make_shared<ShaderProgram>("../src/resources/shaders/terrain.vert", "../src/resources/shaders/terrain.frag");
		m_application.lock()->GetLightManager()->AddShaderProgram(m_shader);
	}
	catch (Exception& e)
	{
		std::cout << "VanillaEngine Exception: " << e.what() << std::endl;
	}
}

void TerrainRenderer::AddTerrainBlock(int _x, int _z, std::shared_ptr<Texture> _blendMap, std::shared_ptr<Texture> _bg, std::shared_ptr<Texture> _r, std::shared_ptr<Texture> _g, std::shared_ptr<Texture> _b)
{
	std::shared_ptr<Terrain> tmp = std::make_shared<Terrain>(_x, _z, _blendMap, _bg, _r, _g, _b);

	m_terrainBlocks.push_back(tmp);
}

void TerrainRenderer::Draw()
{
	SetShaderUniforms();
	
	for (std::list<std::shared_ptr<Terrain>>::iterator i = m_terrainBlocks.begin(); i != m_terrainBlocks.end(); ++i)
	{
		//m_shader->SetUniform("in_Material.diffuse", (*i)->m_tex);
		m_shader->SetUniform("in_Material.blendMap", (*i)->m_blendMap);
		m_shader->SetUniform("in_Material.bg_diffuse", (*i)->m_textures.at(0));
		m_shader->SetUniform("in_Material.r_diffuse", (*i)->m_textures.at(1));
		m_shader->SetUniform("in_Material.g_diffuse", (*i)->m_textures.at(2));
		m_shader->SetUniform("in_Material.b_diffuse", (*i)->m_textures.at(3));
		m_shader->SetUniform("in_ShadowMap", m_application.lock()->GetShadowManager()->GetShadowMap());
	
		glUseProgram(m_shader->GetId());
		SetModelMatrix(*i);
		glBindVertexArray((*i)->m_model_id);
		std::vector<Sampler> samplers = m_shader->GetSamplers();
		for (size_t i = 0; i < samplers.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
	
			if (samplers.at(i).texture)
			{
				glBindTexture(GL_TEXTURE_2D, samplers.at(i).texture->getId());
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		glDrawElements(GL_TRIANGLES, (*i)->GetModelVertexCount(), GL_UNSIGNED_INT, 0);
	
		for (size_t i = 0; i < samplers.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}
}

void TerrainRenderer::SetShaderUniforms()
{
	glm::mat4 view = m_application.lock()->GetCamera()->GetViewMatrix();
	glm::mat4 proj = m_application.lock()->GetCamera()->GetProjectionMatrix();

	m_shader->SetUniform("in_View", view);
	m_shader->SetUniform("in_Projection", proj);
}

void TerrainRenderer::SetModelMatrix(std::shared_ptr<Terrain> _terrain)
{
	glUniformMatrix4fv(glGetUniformLocation(m_shader->GetId(), "in_Model") , 1, GL_FALSE, glm::value_ptr(_terrain->m_model));

	glUniform1f(glGetUniformLocation(m_shader->GetId(), "in_Material.shininess"), 4.0f);
}