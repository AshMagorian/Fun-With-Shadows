#ifndef TEXTURE_H
#define TEXTURE_H

#include "Resource.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

/**
*The texture class is a resource which stores the texture to apply to a model
*/
class Texture : public Resource
{
private:

	GLuint id;
	glm::vec2 size; ///< The size of the texture
protected:
	Texture(int width, int height);
public:
	Texture() {}
	/**
	*\brief Loads the texture from teh path specified
	*/
	Texture(std::string path);

	void SetWrapClampBorder(glm::vec4 _color);
	void SetWrapClampEdge();
	void SetWrapRepeat();

	void SetFilteringLinear();
	void SetFilteringNearest();

	static unsigned char* ExtractImageData(std::string path, int *w, int *h);

	/**
	*\brief Returns the size
	*/
	glm::vec2 getSize() { return size; }
	/**
	*\brief Returns the id
	*/
	GLuint getId() { return id; }
};
#endif