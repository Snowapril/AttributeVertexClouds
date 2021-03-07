#include <AVCRenderer.hpp>
#include <AVCBlockWorld.hpp>
#include <glad/glad.h>

AVCRenderer::AVCRenderer()
{
	//! Do nothing
}

AVCRenderer::~AVCRenderer()
{
	//! Do nothing
}

bool AVCRenderer::OnInitialize(const cxxopts::ParseResult& configure)
{
	if (!AddApplication(std::make_shared<AVCBlockWorld>(), configure))
		return false;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void AVCRenderer::OnCleanUp()
{
	//! Do nothing
}

void AVCRenderer::OnUpdateFrame(double dt)
{
	(void)dt;
}

void AVCRenderer::OnBeginDraw()
{
	//! Do nothing
}

void AVCRenderer::OnEndDraw()
{
	//! Do nothing
}

void AVCRenderer::OnProcessInput(unsigned int key)
{
	(void)key;
}