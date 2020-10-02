#include <VanillaEngine/VanillaEngine.h>

Camera::Camera()
{
	m_viewAngle = 45.0f;
}

Camera::~Camera()
{

}

void Camera::Init(std::weak_ptr<Application> _app, int _w, int _h)
{
	m_app = _app;
	m_windowWidth = _w;
	m_windowHeight = _h;
}

/**
*\brief Throws an exception if the camera wasn't initialised. Calculates the new matrix values
*/
void Camera::UpdateMatrix()
{
	if (!m_currentCamera)
	{
		throw Exception("Camera not found");
	}

	UpdateCameraVectors();

	m_projectionMatrix = glm::perspective(glm::radians(m_viewAngle),
		(float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.f);
	//m_projectionMatrix = glm::ortho(0.0f, (float)_w, 0.0f, (float)_h, -1.0f, 1.0f);
	m_viewMatrix = glm::inverse(m_currentCamera->GetTransform()->GetModelMatrix());

}

void Camera::UpdateCameraVectors()
{
	float yaw = m_currentCamera->GetTransform()->GetRotation().y;
	float pitch = m_currentCamera->GetTransform()->GetRotation().x;

	if (m_isFPSCamera)
		UpdateMouseCameraRotation(&yaw, &pitch);

	m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	m_forwardVector.x = cos(glm::radians(-yaw + 90.0f)) * cos(glm::radians(-pitch));
	m_forwardVector.y = sin(glm::radians(-pitch));
	m_forwardVector.z = sin(glm::radians(-yaw + 90.0f)) * cos(glm::radians(-pitch));
	m_forwardVector = glm::normalize(m_forwardVector);
	m_rightVector = glm::normalize(glm::cross(m_forwardVector, m_upVector));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_upVector = glm::normalize(glm::cross(m_rightVector, m_forwardVector));

}

void Camera::UpdateMouseCameraRotation(float* _yaw, float* _pitch)
{
	float sensitivity = 0.1f;
	float offsetX = m_app.lock()->GetInput()->GetDeltaMouseX() * sensitivity;
	float offsetY = m_app.lock()->GetInput()->GetDeltaMouseY() * sensitivity;

	*_yaw -= offsetX;
	*_pitch += offsetY;

	if (*_pitch > 89.0f) { *_pitch = 89.0f; }
	if (*_pitch < -89.0f) { *_pitch = -89.0f; }

	m_currentCamera->GetTransform()->SetRotation(glm::vec3(*_pitch, *_yaw, 0.0f));
}

void Camera::SetFPSCamera(bool _value)
{
	m_app.lock()->GetInput()->EnableFPSMouse(_value);
	m_isFPSCamera = _value;
}