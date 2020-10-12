#include "LightMovement.h"
#include <iostream>
void LightMovement::OnBegin()
{
	m_transform = GetEntity()->GetTransform();
	m_light = GetApplication()->GetLightManager();
}

void LightMovement::OnTick()
{
	m_x += m_speed;
	glm::vec3 pos = m_transform->GetPos();
	pos.x = m_radius * cos(glm::radians(m_x));
	pos.z = m_radius * sin(glm::radians(m_x));
	m_transform->SetPos(pos);

	glm::vec3 newLightDir = glm::normalize(-pos);
	m_light->SetDirLightDirection(newLightDir);
}