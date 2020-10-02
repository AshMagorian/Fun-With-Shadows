#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

/**
*The environment is a class which stores deltatime whcih can then be accessed by other classes to make sure movement is
*smooth even when the framerate changes
*/
class Environment
{
private:
	unsigned int m_currentTime = 0; ///< current time used for calculating deltaTime
	unsigned int m_lastTime = 0; ///< the previous time used for calculating deltaTime
	float m_deltaTime = 0.0f; ///< Used to make all movement relative to the framerate

public:
	Environment::Environment() { }
	/**
	*\brief Calculates DeltaTime at the start of each frame
	*/
	void StartOfFrame();
	/**
	*\brief Pauses each frame to make sure framerate is smooth
	*/
	void CapFramerate(float _framerate);
	/**
	*\brief Returns deltaTime
	*/
	float GetDeltaTime() { return m_deltaTime; }
};
#endif