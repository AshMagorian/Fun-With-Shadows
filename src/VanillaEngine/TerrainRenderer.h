#ifndef TERRAINRENDERER_H
#define TERRAINRENDERER_H

#include <memory>
#include <list>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Application;
class Terrain;
class ShaderProgram;

class TerrainRenderer
{
	friend class Application;
private:
	std::weak_ptr <Application> m_application;
	std::list<std::shared_ptr<Terrain>> m_terrainBlocks;
	std::shared_ptr<ShaderProgram> m_shader;

	void Init(std::weak_ptr<Application> _app);
	void SetShaderUniforms();
	void SetModelMatrix(std::shared_ptr<Terrain> _terrain);
public:
	TerrainRenderer();

	void AddTerrainBlock(int _x, int _z, std::shared_ptr<Texture> _blendMap, std::shared_ptr<Texture> _bg, std::shared_ptr<Texture> _r, std::shared_ptr<Texture> _g, std::shared_ptr<Texture> _b);
	void Draw();

	std::list<std::shared_ptr<Terrain>> GetTerrain() { return m_terrainBlocks; }
};
#endif