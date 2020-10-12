#include "FirstPersonCameraControls.h"

void FirstPersonCameraControls::OnBegin()
{
	m_Transform = GetEntity()->GetTransform();
	m_Input = GetApplication()->GetInput();
	m_Camera = GetApplication()->GetCamera();
}

void FirstPersonCameraControls::OnTick()
{
	m_forwardVector = m_Camera->GetForwardVector();
	m_rightVector = m_Camera->GetRightVector();
	m_upVector = m_Camera->GetUpVector();

	glm::vec3 totalMovement = glm::vec3(0.0f, 0.0f, 0.0f);
	bool movedFwd = false;
	bool movedHor = false;
	if (m_Input->IsKey('w'))
	{
		movedFwd = !movedFwd;
		totalMovement += m_forwardVector;
	}
	if (m_Input->IsKey('s'))
	{
		movedFwd = !movedFwd;
		totalMovement -= m_forwardVector;
	}
	if (m_Input->IsKey('a')) // move left
	{
		movedHor = !movedHor;
		totalMovement -= m_rightVector;
	}
	if (m_Input->IsKey('d')) // move right
	{
		movedHor = !movedHor;
		totalMovement += m_rightVector;
	}
	if (movedFwd == true || movedHor == true)
	{
		m_Transform->SetPos(m_Transform->GetPos() + (m_movementSpeed * GetApplication()->GetDeltaTime() * glm::normalize(totalMovement)));
	}
}