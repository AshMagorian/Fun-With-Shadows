#include <VanillaEngine/VanillaEngine.h>
#include <thread>
#include <chrono> 

/**
*\brief Calculates DeltaTime at the start of each frame
*/
void Environment::StartOfFrame()
{
	m_currentTime = (int)(glfwGetTime() * 1000);		// Calculate deltatime
	m_deltaTime = (float)(m_currentTime - m_lastTime) / 1000.0f;
}
/**
*\brief Pauses each frame to make sure framerate is smooth
*/
void Environment::CapFramerate(float _framerate)
{
	if (m_deltaTime < (1.0f / _framerate))		//caps framerate
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(int((1.0f / _framerate) - m_deltaTime)*1000));
	}
	m_lastTime = m_currentTime;
}