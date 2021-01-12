#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Texture;

class Terrain
{
	friend class TerrainRenderer;
private:

	const static float size;
	const static int vertexCount = 128;
	float x, z;
	glm::mat4 m_model;
	GLuint m_model_id;
	std::shared_ptr<Texture> m_tex;

	std::shared_ptr<Texture> m_blendMap;
	std::vector<std::shared_ptr<Texture>> m_textures;

	void GenerateTerrain();
	void CreateVAO(std::vector<float> _vert, std::vector<float> _norm, std::vector<float> _tex, std::vector<int> _ind);
	void CreateModelMatrix();
	void SetTextures(std::shared_ptr<Texture> _blendMap, std::shared_ptr<Texture> _bg, std::shared_ptr<Texture> _r, std::shared_ptr<Texture> _g, std::shared_ptr<Texture> _b);


public:
	Terrain(int _x, int _z, std::shared_ptr<Texture> _blendMap, std::shared_ptr<Texture> _bg, std::shared_ptr<Texture> _r, std::shared_ptr<Texture> _g, std::shared_ptr<Texture> _b);

	


	glm::mat4 GetModelMatrix() { return m_model; }
	GLuint GetVaId() { return m_model_id; }
	int GetModelVertexCount();
};
#endif