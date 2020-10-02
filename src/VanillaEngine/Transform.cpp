#include "Transform.h"
#include "Entity.h"
#include <glm/ext.hpp>

void Transform::OnTick()
{
	if (m_rotation.x > 360.0f || m_rotation.x < -360.0f)
	{
		m_rotation.x = 0.0f;
	}
	if (m_rotation.y > 360.0f || m_rotation.y < -360.0f)
	{
		m_rotation.y = 0.0f;
	}
	if (m_rotation.z > 360.0f || m_rotation.z < -360.0f)
	{
		m_rotation.z = 0.0f;
	}
}

/**
*\brief Uses the position, rotation and scale to create a matrix and then returns it as the model matrix
*/
glm::mat4 Transform::GetModelMatrix()
{

	m_model = glm::mat4(1.0f);

	m_model = glm::translate(m_model, m_position);
	m_model = glm::rotate(m_model, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
	m_model = glm::rotate(m_model, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
	m_model = glm::rotate(m_model, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
	m_model = glm::scale(m_model, m_scale);
	return m_model;
}

glm::mat4 Transform::GetRotationMatrix()
{
	m_model = glm::mat4(1.0f);
	m_model = glm::rotate(m_model, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
	m_model = glm::rotate(m_model, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
	m_model = glm::rotate(m_model, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
	return m_model;
}

