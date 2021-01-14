#include "LightMovement.h"
#include <iostream>
void LightMovement::OnBegin()
{
	m_transform = GetEntity()->GetTransform();
	m_light = GetApplication()->GetLightManager();
	m_camTransform = GetApplication()->GetCamera()->GetCurrentCamera()->GetTransform();
}

void LightMovement::OnTick()
{
	m_x += m_speed;
	glm::vec3 pos = m_transform->GetPos();
	pos.y = m_camTransform->GetPos().y + 20.0f;
	pos.x = m_camTransform->GetPos().x + m_radius * cos(glm::radians(m_x));
	pos.z = m_camTransform->GetPos().z + m_radius * sin(glm::radians(m_x));
	m_transform->SetPos(pos);

	glm::vec3 newLightDir = glm::normalize(-pos + m_camTransform->GetPos());
	m_light->SetDirLightDirection(newLightDir);
}