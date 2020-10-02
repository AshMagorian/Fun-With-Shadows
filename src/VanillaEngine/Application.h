#ifndef APPLICATION_H
#define APPLICATION_H
#include <memory>
#include <list>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>



class Entity;
class Camera;
class Environment;
class Input;
class Resources;
class Lights;
class ShadowManager;

class Application
{
private:
	bool running;
	std::list<std::shared_ptr<Entity>> entities; ///< A list of all of teh entities stored in the application
	std::weak_ptr<Application> self; ///< A Stores weak pointer to itself

	std::shared_ptr<Camera> m_mainCamera = std::make_shared<Camera>(); ///< A pointer to the main camera in the game
	std::shared_ptr<Environment> m_time = std::make_shared<Environment>(); ///< A pointer to the environment object so DeltaTime can be accessed
	std::shared_ptr<Input> m_input = std::make_shared<Input>();
	std::shared_ptr<Resources> m_resourceManager = std::make_shared<Resources>(); ///< A pointer to the resource manager for accesibility
	std::shared_ptr<Lights> m_lightManager = std::make_shared<Lights>();
	std::shared_ptr<ShadowManager> m_shadowManager = std::make_shared<ShadowManager>();

	GLFWwindow* m_window = nullptr;

	int m_windowWidth, m_windowHeight;

	void UpdateCameraData();
	void RenderScene();

public:
	Application();
	~Application();
	static std::shared_ptr<Application> const Init(int _w, int _h);
	void Run();
	void Stop();

	static void error_callback(int error, const char* description);

	std::shared_ptr<Camera> GetCamera() { return m_mainCamera; }
	std::shared_ptr<Resources> GetResourceManager() { return m_resourceManager; }
	std::shared_ptr<Lights> GetLightManager() { return m_lightManager; }
	std::shared_ptr<Input> GetInput() { return m_input; }
	std::shared_ptr<ShadowManager> GetShadowManager() { return m_shadowManager; }
	float GetDeltaTime();
	std::shared_ptr<Entity> AddEntity();

};
#endif