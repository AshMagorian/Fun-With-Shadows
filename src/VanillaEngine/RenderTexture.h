#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include "Texture.h"

class RenderTexture : public Texture
{
private:
	GLuint fboId;
public:
	RenderTexture();
	RenderTexture(int _width, int _height);
	~RenderTexture() { glDeleteFramebuffers(1, &fboId); }

	void Init();
	void InitDepthOnly();

	GLuint GetFboId() { return fboId; }

	void Clear();
};
#endif