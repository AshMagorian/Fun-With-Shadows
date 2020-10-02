#ifndef INPUT_H
#define INPUT_H
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Input
{
	friend class Application;
private:
	static std::vector<int> m_keys; ///< Keys which are held down
	static std::vector<int> m_pressedKeys; ///<Keys which have been pressed this frame
	static std::vector<int> m_releasedKeys; ///< Keys which have been released this frame

	void static KeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
	void static MouseCallback(GLFWwindow* _window, double xpos, double ypos);
	void static ClearKeys();


	void ResetDeltaMouseValues() { m_deltaMouseX = 0.0f; m_deltaMouseY = 0.0f; }

	static bool m_FPSMouseEnabled;
	static bool m_firstFrame;
	static float m_deltaMouseX, m_deltaMouseY;
	static double m_lastX, m_lastY;

	GLFWwindow* m_window = nullptr;
public:
	bool IsKey(int _key);
	bool IsKeyPressed(int _key);
	bool IsKeyReleased(int _key);

	void EnableFPSMouse(bool _value);

	float GetDeltaMouseX() { return m_deltaMouseX; }
	float GetDeltaMouseY() { return m_deltaMouseY; }

};
#endif