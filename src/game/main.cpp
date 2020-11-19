#include <VanillaEngine/VanillaEngine.h>

#include "FirstPersonCameraControls.h"
#include "LightMovement.h"

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
	application->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/solidColorShader.txt", "solid_color_shader");
	application->GetResourceManager()->CreateResource<VertexArray>("../src/resources/Mountain.obj", "mountain");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/grass.jpg", "grass_tex");

	application->GetResourceManager()->CreateResource<Texture>("../src/resources/fantasyDiff.jpg", "fantasy_tex");
	//application->GetResourceManager()->CreateResource<VertexArray>("../src/resources/fantasy.obj", "fantasy");

	application->getSkybox()->CreateSkybox("space",
		"../src/resources/textures/space_right.png",
		"../src/resources/textures/space_left.png",
		"../src/resources/textures/space_up.png",
		"../src/resources/textures/space_down.png",
		"../src/resources/textures/space_back.png",
		"../src/resources/textures/space_front.png" );

	//application->GetLightManager()->AddShaderProgram(application->GetResourceManager()->LoadFromResources<ShaderProgram>("lighting_shader"));
	application->GetLightManager()->AddShaderProgram(application->GetResourceManager()->LoadFromResources<ShaderProgram>("phong_shadow_shader"));

	std::shared_ptr<Entity> camera = application->AddEntity();
	camera->AddComponent<FirstPersonCameraControls>();
	application->GetCamera()->SetCurrentCamera(camera);
	camera->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
	application->GetCamera()->SetFPSCamera(true);

	std::shared_ptr<Entity> sun = application->AddEntity();
	sun->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("solid_color_shader"),
									application->GetResourceManager()->LoadFromResources<VertexArray>("cube"),
									glm::vec4(1.0f, 0.4f, 1.0f, 1.0f));
	sun->GetTransform()->SetPos(glm::vec3(0.0f, 17.0f, 0.0f));
	sun->AddComponent<LightMovement>();
	sun->GetComponent<Renderer>()->ToggleShadow(false);


	std::shared_ptr<Entity> testBox = application->AddEntity();
	testBox->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("phong_shadow_shader"),
									application->GetResourceManager()->LoadFromResources<VertexArray>("cube") ,
									application->GetResourceManager()->LoadFromResources<Texture>("orange_tex"));
	testBox->GetTransform()->SetPos(glm::vec3(0.0f, -5.0f, 0.0f));

	//std::shared_ptr<Entity> mountain = application->AddEntity();
	//mountain->AddComponent<Renderer>(application->GetResourceManager()->LoadFromResources<ShaderProgram>("phong_shadow_shader"),
	//								application->GetResourceManager()->LoadFromResources<VertexArray>("fantasy"),
	//								application->GetResourceManager()->LoadFromResources<Texture>("fantasy_tex"));
	//mountain->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
	//mountain->GetTransform()->SetScale(glm::vec3(100.0f, 100.0f, 100.0f));
	//mountain->GetTransform()->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));


	/**
	*Runs the game loop from application
	*/
	application->Run();
	return 0;
}