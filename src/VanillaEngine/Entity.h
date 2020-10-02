#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <list>
#include <string>
#include <typeinfo>

class Application;
class Component;
class Transform;

class Entity
{
	friend class Application;

private:
	std::weak_ptr<Application> m_application; ///< A reference to the application
	std::list<std::shared_ptr<Component>> m_components; ///< The collection of components stored in the entity
	std::weak_ptr<Entity> m_self; ///< A reference to itself
	std::weak_ptr<Transform> m_transform; ///< The position, rotation adn scale of the entity

	void Tick();
	void Display();
public:
	Entity();
	~Entity();

	std::shared_ptr<Application> GetApplication() { return m_application.lock(); }

	template <typename T, typename... A>
	std::shared_ptr<T> AddComponent(A... args)
	{
		std::shared_ptr<T> rtn = std::make_shared<T>();
		rtn->m_entity = m_self;
		rtn->m_began = false;
		m_components.push_back(rtn);

		rtn->OnInit(args...);

		return rtn;
	}

	template <typename T>
	std::shared_ptr<T> GetComponent()
	{
		std::shared_ptr<T> rtn;
		try
		{
			for (std::list<std::shared_ptr<Component>>::iterator i = m_components.begin(); i != m_components.end(); ++i)
			{
				rtn = std::dynamic_pointer_cast<T>(*i);
				if (rtn)
				{
					return rtn;
				}
			}
			throw Exception("Componenet " + std::string(typeid(T).name()) + " could not be found");
		}
		catch (Exception& e) { std::cout << "VanillaEngine Exception: " << e.what() << std::endl; }
		return NULL;
	}

	std::list<std::shared_ptr<Component>> GetComponents() { return m_components; }
	std::shared_ptr<Transform> GetTransform() { return m_transform.lock(); }
};
#endif