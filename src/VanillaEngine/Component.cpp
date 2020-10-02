#include "VanillaEngine.h"

Component::Component() {}
Component::~Component() {}

std::shared_ptr<Entity> Component::GetEntity()
{
	return m_entity.lock();
}

std::shared_ptr<Application> Component::GetApplication()
{
	return GetEntity()->GetApplication();
}

