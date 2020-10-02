#include <VanillaEngine/VanillaEngine.h>

RenderTexture::RenderTexture()
{

}
RenderTexture::RenderTexture(int _width, int _height) : Texture(_width, _height)
{
	glGenFramebuffers(1, &fboId);
}

void RenderTexture::Init()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->getId(), 0);

	GLuint rbo = 0;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->getSize().x, this->getSize().y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void RenderTexture::InitDepthOnly()
{
	glBindTexture(GL_TEXTURE_2D, this->getId());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->getSize().x, this->getSize().y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->getId(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}