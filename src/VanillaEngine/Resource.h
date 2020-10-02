#ifndef RESOURCE_H
#define RESOURCE_H
#include <string>

/**
*An abstract class for resources loaded in by the user (Models, textures, shader programs and sound)
*/
class Resource
{
private:
	std::string m_path; ///< The directory path for the resource
	std::string m_name; ///< the name given to the resource
public:
	/**
	*\brief This only exists as there needs to be a virtual functino to make a class abstract
	*/
	virtual ~Resource() {}

	std::string GetPath() { return m_path; }
	std::string GetName() { return m_name; }
	void SetPath(std::string _path) { m_path = _path; }
	void SetName(std::string _name) { m_name = _name; }
};
#endif