#ifndef RESOURCES_H
#define RESOURCES_H

class Resources
{
private:
	std::list<std::shared_ptr<Resource>> m_resources; ///< The list of resources

public:
	Resources() {}
	/**
	*Takes a string and then compares it with the path of every resource. When it finds the correct
	*resource it returns it using dynamic pointer cast. If it can't find the resource it throws and catches
	*an exception to prevent the game from crashing and displays an error message
	*/
	template<typename T>
	std::shared_ptr<T> LoadFromResources(std::string _name)
	{
		try
		{
			std::shared_ptr<T> rtn;
			for (std::list<std::shared_ptr<Resource>>::iterator i = m_resources.begin(); i != m_resources.end(); ++i)
			{
				if (_name == (*i)->GetName())
				{
					rtn = std::dynamic_pointer_cast<T>(*i);
					if (rtn)
					{
						std::cout << (*i)->GetName() << " loaded" << std::endl;
						return rtn;
					}
				}
			}
			throw Exception(_name + " cannot be loaded from Resources");
		}
		catch (Exception& e)
		{
			std::cout << "VanillaEngine Exception: " << e.what() << std::endl;
		}
		catch (std::exception& e)
		{
			std::cout << "exception: " << e.what() << std::endl;
		}
		return NULL;
	}

	/**
	*Takes the path provided and creates the resource defined. This resource is then
	*pushed back onto the list
	*/
	template<typename T>
	std::shared_ptr<T> CreateResource(std::string _path, std::string _name)
	{
		std::shared_ptr<T> resource;
		try
		{
			resource = std::make_shared<T>(_path);
		}
		catch (Exception& e)
		{
			std::cout << "myEngine Exception: " << e.what() << std::endl;
			return NULL;
		}
		resource->SetPath(_path);
		resource->SetName(_name);
		m_resources.push_back(resource);
		std::cout << _name << " created" << std::endl;
		return resource;
	}

	template<typename T>
	std::shared_ptr<T> CreateResource(std::shared_ptr<T> _res, std::string _name)
	{
		try
		{
			if (!_res)
			{
				throw Exception(_name + " cannot be saved to resources as it does not exist");
			}
		}
		catch (Exception& e)
		{
			std::cout << "myEngine Exception: " << e.what() << std::endl;
			return;
		}
		_res->SetPath(_name);
		_res->SetName(_name);
		m_resources.push_back(_res);
		std::cout << _name << " created" << std::endl;
	}
};
#endif