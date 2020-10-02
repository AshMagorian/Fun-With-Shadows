#include <VanillaEngine/VanillaEngine.h>

#include "FirstPersonCameraControls.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

int main(int argc, char *argv[])
{
	std::shared_ptr<Application> application = Application::Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	application->GetResourceManager()->CreateResource<Texture>("../src/resources/orange.png", "orange_tex");
	application->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/lightingShader.txt", "lighting_shader");
	application->GetResourceManager()->CreateResource<VertexArray>("../src/resources/Cube.obj", "cube");
	application->GetResourceManager()->CreateResource<VertexArray>("../src/resources/Plane.obj", "plane");
	application->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/phongShadowShader.txt", "phong_shadow_shader");

	//application->GetLightManager()->AddShaderProgram(application->GetResourceManager()->LoadFromResources<ShaderProgram>("lighting_shader"));
	application->GetLightManager()->AddShaderProgram(application->GetResourceManager()->LoadFromResources<ShaderProgram>("phong_shadow_shader"));

	std::shared_ptr<Entity> camera = application->AddEntity();
	camera->AddComponent<FirstPersonCameraControls>();
	application->GetCamera()->SetCurrentCamera(camera);
	camera->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
	application->GetCamera()->SetFPSCamera(true);

	std::shared_ptr<Entity> testBox = application->AddEntity();
	testBox->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("phong_shadow_shader"),
									application->GetResourceManager()->LoadFromResources<VertexArray>("cube") ,
									application->GetResourceManager()->LoadFromResources<Texture>("orange_tex"));

	std::shared_ptr<Entity> floor = application->AddEntity();
	floor->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("phong_shadow_shader"),
									application->GetResourceManager()->LoadFromResources<VertexArray>("plane"),
									application->GetResourceManager()->LoadFromResources<Texture>("orange_tex"));
	floor->GetTransform()->SetPos(glm::vec3(0.0f, -4.0f, 0.0f));
	floor->GetTransform()->SetScale(glm::vec3(3.0f, 1.0f, 3.0f));

	/**
	*Runs the game loop from application
	*/
	application->Run();
	return 0;
}