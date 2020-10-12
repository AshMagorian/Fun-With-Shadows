#include <VanillaEngine/VanillaEngine.h>

class LightMovement : public Component
{
private:
	float m_radius = 5.0f;
	float m_speed = 1.0f;
	double m_x = 0.0f;
	std::shared_ptr<Transform> m_transform;
	std::shared_ptr<Lights> m_light;

public:
	void OnBegin();
	void OnTick();
};