#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Component.h"
#include "glm/glm.hpp"

/**
*The transform component is included in every entity. About 90% of entities will need to access transform and there
*is negligible computational cost in the remaining 10% of entities accessing it. It stores all of the necessary data
*to create the model matrix
*/
class Transform : public Component
{
private:

	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f); ///< The position of the entity
	glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);///< The rotation of the entity
	glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f); ///< The scale of the entity
public:

	Transform() {}
	~Transform() {}

	void OnTick();


	glm::vec3 GetPos() { return m_position; }
	glm::vec3 GetRotation() { return m_rotation; }
	glm::vec3 GetScale() { return m_scale; }

	void SetPos(glm::vec3 _pos) { m_position = _pos; }
	void SetRotation(glm::vec3 _rot) { m_rotation = _rot; }
	void SetScale(glm::vec3 _sca) { m_scale = _sca; }

	glm::mat4 GetModelMatrix();
	glm::mat4 GetRotationMatrix();

protected:
	glm::mat4 m_model; ///< The model matrix
};
#endif