#ifndef CAMERA_H
#define CAMERA_H
#include "Component.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
	/**
	*This camera class stores the vital data of the main camera of the scene. It stores a pointer to a camera defined
	*in the game itself.
	*/
class Camera
{
	friend class Application;
private:
	void Init(std::weak_ptr<Application> _app, int _w, int _h);

	int m_windowWidth, m_windowHeight;
	float m_viewAngle; ///< The viewing angle of the camera
	bool m_isFPSCamera = false;

	glm::mat4 m_viewMatrix; ///< The view matrix of the main camera
	glm::mat4 m_projectionMatrix; ///< The projection matrix of the main camera

	std::shared_ptr<Entity> m_currentCamera; ///< A pointer to the main camera set from the main game

	void UpdateCameraVectors();
	void UpdateMouseCameraRotation(float* _yaw, float* _pitch);

	glm::vec3 m_upVector;
	glm::vec3 m_rightVector;
	glm::vec3 m_forwardVector;

	std::weak_ptr<Application> m_app;

public:
	Camera();
	~Camera();
	/**
	*\brief Updates the 2 matrix values
	*/
	void UpdateMatrix();
	/**
	*\brief Sets the new view angle
	*/
	void SetViewAngle(float _angle) { m_viewAngle = _angle; }
	/**
	*\brief Sets which camera to render from
	*/
	void SetCurrentCamera(std::shared_ptr<Entity> _cam) { m_currentCamera = _cam; }
	/**
	*\brief Gets the main camera
	*/
	std::shared_ptr<Entity> GetCurrentCamera() { return m_currentCamera; }
	/**
	*\brief Returns the view matrix
	*/
	glm::mat4 GetViewMatrix() { return m_viewMatrix; }
	/**
	*\brief Returns the projection matrix
	*/
	glm::mat4 GetProjectionMatrix() { return m_projectionMatrix; }
	float GetAspectRatio() { return (float)m_windowWidth / (float)m_windowHeight; }
	std::shared_ptr<Application> GetApplication() { return m_app.lock(); }

	float GetViewAngle() { return m_viewAngle; }
	glm::vec3 GetUpVector() { return m_upVector; }
	glm::vec3 GetRightVector() { return m_rightVector; }
	glm::vec3 GetForwardVector() { return m_forwardVector; }

	void SetFPSCamera(bool _value);
};
#endif