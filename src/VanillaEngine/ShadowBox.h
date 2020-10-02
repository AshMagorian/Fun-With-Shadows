#ifndef SHADOWBOX_H
#define SHADOWBOX_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

class Camera;

/*
Represents the cuboid in which shadows are rendered
*/
class ShadowBox
{
	friend class ShadowManager;
private:
	bool m_first = true;
	glm::vec4 m_upVector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	glm::vec4 m_forwardVector = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	float m_nearPlaneDistance = 0.1f;
	float m_shadowDistance = 100.0f;

	float m_minX, m_maxX;
	float m_minY, m_maxY;
	float m_minZ, m_maxZ;
	float m_farWidth, m_farHeight, m_nearWidth, m_nearHeight;

	glm::mat4 m_lightViewMatrix;
	std::shared_ptr<Camera> m_camera;

	void SetCamera(std::shared_ptr<Camera> _cam) { m_camera = _cam; }
	void CalcFrustrumWidthsAndHeights();
	void CalcInitialLightViewMatrix();
	void CalcFrustrumPoints(glm::vec3 _fwdVector, glm::vec3 _fwdNear, glm::vec3 _fwdFar, std::vector<glm::vec4>camFrustrumPoints);
	glm::vec4 CalcFrustrumCorner(glm::vec3 _fwd, glm::vec3 _horDist, glm::vec3 _verDist);

public:
	ShadowBox();

	void Update();

	void SetLightViewMatrix(glm::mat4 _mtx) { m_lightViewMatrix = _mtx; }

};
#endif