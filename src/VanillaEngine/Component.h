#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <string>
#include <iostream>

class Entity;
class Application;

class Component
{
	friend class Entity;

private:
	std::weak_ptr<Entity> m_entity; ///< a reference to the entity which owns this component
	bool m_began; ///< Determines if onBegin should run
	/**
	*\brief Runs on startup
	*/
	virtual void OnInit() {}
	/**
	*\brief Runs on the first frame
	*/
	virtual void OnBegin() {}
	/**
	*\brief Runs every frame
	*/
	virtual void OnTick() {}
	/**
	*\brief Calls the rendering functions (Only useful for the Render class)
	*/
	virtual void OnDisplay() {}

public:
	Component();
	virtual ~Component();

	std::shared_ptr<Entity> GetEntity();
	std::shared_ptr<Application> GetApplication();
};
#endif