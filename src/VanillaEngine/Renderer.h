#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class ShaderProgram;
class VertexArray;
class Texture;

struct ShadowData
{
	std::shared_ptr<Transform> transform;
	std::shared_ptr<VertexArray> VA;
};

class Renderer : public Component
{
private:
	std::shared_ptr<ShaderProgram> m_shaderProgram;
	std::shared_ptr<VertexArray> m_va;
	std::shared_ptr<Texture> m_tex;
	std::shared_ptr<Camera> m_cam;
	glm::vec4 m_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	ShadowData m_shadowData;

public:
	Renderer();
	~Renderer();

	void OnInit() {}
	void OnInit(std::shared_ptr<ShaderProgram> _shader, std::shared_ptr<VertexArray> _va, std::shared_ptr<Texture> _tex);
	void OnInit(std::shared_ptr<ShaderProgram> _shader, std::shared_ptr<VertexArray> _va, glm::vec4 _color);

	void OnTick();
	void OnDisplay();

	void ToggleShadow(bool _value);

	void SetMesh(std::shared_ptr<VertexArray> _va) { m_va = _va; }
	void SetTexture(std::shared_ptr<Texture> _tex) { m_tex = _tex; }
	void SetShader(std::shared_ptr<ShaderProgram> _shader) { m_shaderProgram = _shader; }
	void SetCamera(std::shared_ptr<Camera> _cam) { m_cam = _cam; }

};
#endif