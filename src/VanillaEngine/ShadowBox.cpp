#include <VanillaEngine/VanillaEngine.h>

ShadowBox::ShadowBox()
{

}

void ShadowBox::CalcFrustrumWidthsAndHeights()
{
	m_farHeight = 2.0f * m_shadowDistance * tan(glm::radians(m_camera->GetViewAngle()) * 0.5f);
	m_nearHeight = 2.0f * m_nearPlaneDistance * tan(glm::radians(m_camera->GetViewAngle()) * 0.5f);
	m_farWidth = m_farHeight * m_camera->GetAspectRatio();
	m_nearWidth = m_nearHeight * m_camera->GetAspectRatio();
}

void ShadowBox::CalcInitialLightViewMatrix()
{
	glm::vec3 lightDir = m_camera->GetApplication()->GetLightManager()->GetDirectionalLight()->direction;
	glm::vec3 lightPos = 3.0f * glm::normalize(-lightDir);
	m_lightViewMatrix = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void ShadowBox::Update()
{
	if (m_first)
	{
		CalcInitialLightViewMatrix();
		m_first = false;
	}
	glm::vec3 fwdVector = m_camera->GetForwardVector();
	glm::vec3 camPos = m_camera->GetCurrentCamera()->GetTransform()->GetPos();
	glm::vec3 fwdFar = camPos + (fwdVector * m_shadowDistance);
	glm::vec3 fwdNear = camPos + (fwdVector * m_nearPlaneDistance);
	std::vector<glm::vec4> camFrustrumPoints;
	CalcFrustrumPoints(fwdVector, fwdNear, fwdFar, camFrustrumPoints);
}

void ShadowBox::CalcFrustrumPoints(glm::vec3 _fwdVector, glm::vec3 _fwdNear, glm::vec3 _fwdFar, std::vector<glm::vec4> _camFrustrumPoints)
{
	glm::vec3 upVector = m_camera->GetUpVector();
	glm::vec3 rightVector = m_camera->GetRightVector();
	glm::vec3 farRight = rightVector * (m_farWidth * 0.5f);
	glm::vec3 farUp = upVector * (m_farHeight * 0.5f);
	glm::vec3 nearRight = rightVector * (m_nearWidth * 0.5f);
	glm::vec3 nearUp = upVector * (m_nearHeight * 0.5f);
	_camFrustrumPoints.resize(8);
	_camFrustrumPoints.at(0) = CalcFrustrumCorner(_fwdNear, -nearRight, nearUp);
	_camFrustrumPoints.at(1) = CalcFrustrumCorner(_fwdNear, nearRight, nearUp);
	_camFrustrumPoints.at(2) = CalcFrustrumCorner(_fwdNear, nearRight, -nearUp);
	_camFrustrumPoints.at(3) = CalcFrustrumCorner(_fwdNear, -nearRight, -nearUp);
	_camFrustrumPoints.at(4) = CalcFrustrumCorner(_fwdFar,  -farRight, farUp);
	_camFrustrumPoints.at(5) = CalcFrustrumCorner(_fwdFar, farRight, farUp);
	_camFrustrumPoints.at(6) = CalcFrustrumCorner(_fwdFar, farRight, -farUp);
	_camFrustrumPoints.at(7) = CalcFrustrumCorner(_fwdFar, -farRight, -farUp);
	//for (int i = 0; i < 8; i++)
	//{
	//	std::cout << i << ": " << _camFrustrumPoints.at(i).x << ", " << _camFrustrumPoints.at(i).y << ", " << _camFrustrumPoints.at(i).z << std::endl;
	//}
	//std::cout << std::endl;
}

/*
Calculates the frustum point in light space
*/
glm::vec4 ShadowBox::CalcFrustrumCorner(glm::vec3 _fwd, glm::vec3 _horDist, glm::vec3 _verDist)
{
	glm::vec3 p = _fwd + _horDist + _verDist;
	glm::vec4 rtn = m_lightViewMatrix * glm::vec4(p, 1.0f);
	return rtn;
}