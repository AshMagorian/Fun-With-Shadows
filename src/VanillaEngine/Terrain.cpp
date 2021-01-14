#include <VanillaEngine/VanillaEngine.h>

#include "stb_image.h"

const float Terrain::size = 100.0f;
const float Terrain::m_maxHeight = 10.0f;
const float Terrain::m_maxPixelColour = 256.0f;

Terrain::Terrain(int _x, int _z, std::shared_ptr<Texture> _blendMap, std::shared_ptr<Texture> _bg, std::shared_ptr<Texture> _r, std::shared_ptr<Texture> _g, std::shared_ptr<Texture> _b)

{
	SetTextures(_blendMap, _bg, _r, _g, _b);
	x = _x * size;
	z = _z * size;
	GenerateTerrain();
	CreateModelMatrix();
}

void Terrain::SetTextures(std::shared_ptr<Texture> _blendMap, std::shared_ptr<Texture> _bg, std::shared_ptr<Texture> _r, std::shared_ptr<Texture> _g, std::shared_ptr<Texture> _b)
{
	m_blendMap = _blendMap;
	m_textures.resize(4);
	m_textures.at(0) = _bg;
	m_textures.at(1) = _r;
	m_textures.at(2) = _g;
	m_textures.at(3) = _b;
}

void Terrain::GenerateTerrain()
{
	//Read the depth texture
	int w, h;
	int channels = 0;
	unsigned char *data = stbi_load("../src/resources/heightmap.png", &w, &h, &channels, 4);
	//Set the vertex count to the pixel height of the image
	vertexCount = h;

	int count = vertexCount * vertexCount;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<int> indices;

	for (int i = 0; i<vertexCount; ++i)
	{
		for (int j = 0; j < vertexCount; ++j)
		{
			float j_unitInterval = (float)j / ((float)vertexCount - 1.0f);
			float i_unitInterval = (float)i / ((float)vertexCount - 1.0f);

			GetTerrainHeight(j, i, data);

			vertices.push_back(j_unitInterval * size);
			vertices.push_back(GetTerrainHeight(j, i, data));
			vertices.push_back(i_unitInterval * size);
			glm::vec3 normal = CalculateNormal(j, i, data);
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);
			texCoords.push_back(j_unitInterval);
			texCoords.push_back(i_unitInterval);
		}
	}
	int pointer = 0;
	for (int gz = 0; gz < vertexCount - 1; gz++) 
	{
		for (int gx = 0; gx < vertexCount - 1; gx++) 
		{
			int topLeft = (gz * vertexCount) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * vertexCount) + gx;
			int bottomRight = bottomLeft + 1;
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	free(data);

	CreateVAO(vertices, normals, texCoords, indices);
}

float Terrain::GetTerrainHeight(int x, int y, unsigned char* data)
{
	//Check if the selected pixel is in bounds
	if (x < 0 || x >= vertexCount || y < 0 || y >= vertexCount)
		return 0.0f;
	float height = data[4 * (y * vertexCount + x) + 0];
	height -= (m_maxPixelColour / 2.0f);
	height /= (m_maxPixelColour / 2.0f);
	height *= m_maxHeight;

	//std::cout << height << std::endl;
	return height;
}

glm::vec3 Terrain::CalculateNormal(int x, int y, unsigned char* data)
{
	float l = GetTerrainHeight(x - 1, y, data);
	float r = GetTerrainHeight(x + 1, y, data);
	float d = GetTerrainHeight(x, y - 1, data);
	float u = GetTerrainHeight(x, y + 1, data);
	glm::vec3 normal = glm::vec3(l - r, 2.0f, d - u);
	glm::normalize(normal);
	return normal;
}

void Terrain::CreateVAO(std::vector<float> _vert, std::vector<float> _norm, std::vector<float> _tex, std::vector<int> _ind)
{
	glGenVertexArrays(1, &m_model_id);
	glBindVertexArray(m_model_id);

	//create and attach vertex buffer
	GLuint vector_id;
	glGenBuffers(1, &vector_id);
	glBindBuffer(GL_ARRAY_BUFFER, vector_id);
	glBufferData(GL_ARRAY_BUFFER, _vert.size() * sizeof(float), &_vert.at(0), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GLfloat), (void*)0);

	//create and attach normal buffer
	GLuint normal_id;
	glGenBuffers(1, &normal_id);
	glBindBuffer(GL_ARRAY_BUFFER, normal_id);
	glBufferData(GL_ARRAY_BUFFER, _norm.size() * sizeof(float), &_norm.at(0), GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GLfloat), (void*)0);

	//create and attach texCoord buffer
	GLuint tex_id;
	glGenBuffers(1, &tex_id);
	glBindBuffer(GL_ARRAY_BUFFER, tex_id);
	glBufferData(GL_ARRAY_BUFFER, _tex.size() * sizeof(float), &_tex.at(0), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		2 * sizeof(GLfloat), (void*)0);

	//create and attach index buffer
	GLuint index_id;
	glGenBuffers(1, &index_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _ind.size() * sizeof(int), &_ind.at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Terrain::CreateModelMatrix()
{
	glm::mat4 model(1.0f);
	glm::vec3 position(x, 0.0f, z);
	model = glm::translate(model, position);
	m_model = model;
}

int Terrain::GetModelVertexCount()
{
	return (vertexCount - 1) * (vertexCount - 1) * 6;
}