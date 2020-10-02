#include "VanillaEngine.h"

Entity::Entity()
{

}

Entity::~Entity()
{

}

void Entity::Tick()
{
	for (std::list < std::shared_ptr<Component>>::iterator i = m_components.begin(); i != m_components.end(); ++i)
	{
		if ((*i)->m_began == false)
		{
			(*i)->OnBegin();
			(*i)->m_began = true;
		}
		(*i)->OnTick();
	}
}
/**
*\brief Runs through each component's display function
*/
void Entity::Display()
{
	for (std::list<std::shared_ptr<Component>>::iterator i = m_components.begin(); i != m_components.end(); ++i)
	{
		(*i)->OnDisplay();
	}
}