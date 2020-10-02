#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
/**
*The vertex buffer stores a particular type of data to be stored in vertex array
*/
class VertexBuffer
{
private:
	GLuint id;
	int VBcomponents;
	std::vector<GLfloat> data;
	bool dirty;

public:
	VertexBuffer();

	void add(glm::vec2 value);
	void add(glm::vec3 value);
	void add(glm::vec4 value);
	void SetData(std::vector<GLfloat> _data, int _vbComponents);

	int GetComponents() { return VBcomponents; }
	int GetDataSize() { return data.size(); }
	GLuint GetId();
};
#endif