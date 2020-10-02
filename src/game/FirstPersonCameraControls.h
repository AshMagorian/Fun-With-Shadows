#include <VanillaEngine/VanillaEngine.h>

class FirstPersonCameraControls : public Component
{
private:
	std::shared_ptr<Transform> m_Transform; ///< The transform component 
	std::shared_ptr<Input> m_Input; ///< The input component of the engine
	std::shared_ptr<Camera> m_Camera;

	glm::vec3 m_forwardVector = glm::vec3(0.0f, 0.0f, -1.0f); ///< A vector which always points in the direction the camera is facing
	glm::vec3 m_rightVector; ///< A vector which points to the rigth of where the camera is facing
	glm::vec3 m_upVector;

	float m_movementSpeed = 10.0f; ///< The speed in which the player moves forward

public:
	void OnBegin();
	void OnTick();
};