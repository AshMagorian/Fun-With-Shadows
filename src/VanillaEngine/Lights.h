#ifndef LIGHTS_H
#define LIGHTS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <list>

class PointLight;
class SpotLight;
class ShaderProgram;
class Application;

struct DirectionalLight
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 direction;
};

class Lights
{
	friend class Application;
private:
	std::shared_ptr<DirectionalLight> m_directionalLight = std::make_shared<DirectionalLight>();
	std::list<std::shared_ptr<PointLight>> m_pointLights;
	std::list<std::shared_ptr<SpotLight>> m_spotLights;

	std::list<std::shared_ptr<ShaderProgram>> m_shaderPrograms;

	std::weak_ptr<Application> m_application;
	void Init(std::weak_ptr<Application> _app) { m_application = _app; }
public:
	Lights();
	std::shared_ptr<DirectionalLight> GetDirectionalLight() { return m_directionalLight; }
	void SetDirLightDirection(glm::vec3 _dir) { m_directionalLight->direction = _dir; }

	void AddShaderProgram(std::shared_ptr<ShaderProgram> _shader) { m_shaderPrograms.push_back(_shader); }

	void UpdateLightShaderValues();
	void UpdateShadowValues();

	void AddPointLight(std::shared_ptr<Entity> _entity);
	void AddSpotLight(std::shared_ptr<Entity> _entity);

};
#endif