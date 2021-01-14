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
	application->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/phongShadowShader.txt", "phong_shadow_shader");
	application->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/solidColorShader.txt", "solid_color_shader");


	application->GetResourceManager()->CreateResource<Texture>("../src/resources/blendMap.png", "blendMap");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/grass.jpg", "grass_tex");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/dirt.png", "dirt_tex");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/flowers.png", "flowers_tex");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/path.jpg", "path_tex");

	application->GetSkybox()->CreateSkybox("bg",
		"../src/resources/textures/right.jpg",
		"../src/resources/textures/left.jpg",
		"../src/resources/textures/top.jpg",
		"../src/resources/textures/bottom.jpg",
		"../src/resources/textures/back.jpg",
		"../src/resources/textures/front.jpg");



	//application->GetLightManager()->AddShaderProgram(application->GetResourceManager()->LoadFromResources<ShaderProgram>("lighting_shader"));
	application->GetLightManager()->AddShaderProgram(application->GetResourceManager()->LoadFromResources<ShaderProgram>("phong_shadow_shader"));

	std::shared_ptr<Entity> camera = application->AddEntity();
	camera->AddComponent<FirstPersonCameraControls>();
	application->GetCamera()->SetCurrentCamera(camera);
	camera->GetTransform()->SetPos(glm::vec3(15.0f, 5.0f, 15.0f));
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
	testBox->GetTransform()->SetPos(glm::vec3(20.0f, 5.0f, 20.0f));


	application->GetTerrainRenderer()->AddTerrainBlock(0, 0,	application->GetResourceManager()->LoadFromResources<Texture>("blendMap"), 
																application->GetResourceManager()->LoadFromResources<Texture>("grass_tex"), 
																application->GetResourceManager()->LoadFromResources<Texture>("dirt_tex"), 
																application->GetResourceManager()->LoadFromResources<Texture>("flowers_tex"), 
																application->GetResourceManager()->LoadFromResources<Texture>("path_tex"));



	/**
	*Runs the game loop from application
	*/
	application->Run();
	return 0;
}