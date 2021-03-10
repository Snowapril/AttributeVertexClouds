#include <AVCBlockWorld.hpp>
#include <GL3/Window.hpp>
#include <GL3/PerspectiveCamera.hpp>
#include <GL3/Shader.hpp>
#include <GL3/Texture.hpp>
#include <GL3/AssetLoader.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <random>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

AVCBlockWorld::AVCBlockWorld()
{
	//! Do nothing
}

AVCBlockWorld::~AVCBlockWorld()
{
	//! Do nothing
}

bool AVCBlockWorld::OnInitialize(std::shared_ptr<GL3::Window> window, const cxxopts::ParseResult& configure)
{
	(void)configure;

	auto defaultCam = std::make_shared<GL3::PerspectiveCamera>();

	if (!defaultCam->SetupUniformBuffer())
		return false;

	defaultCam->SetupCamera(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	defaultCam->SetProperties(window->GetAspectRatio(), 60.0f, 0.1f, 100.0f);
	defaultCam->UpdateMatrix();

	AddCamera(std::move(defaultCam));

	auto defaultShader = std::make_shared<GL3::Shader>();
	if (!defaultShader->Initialize({ {GL_VERTEX_SHADER, RESOURCES_DIR "/shaders/vertex.glsl"},
									 {GL_GEOMETRY_SHADER, RESOURCES_DIR "/shaders/avc_geometry.glsl"},
									 {GL_FRAGMENT_SHADER, RESOURCES_DIR "/shaders/output.glsl"} }))
		return false;

	defaultShader->BindUniformBlock("CamMatrices", 0);
	defaultShader->BindFragDataLocation("fragColor", 0);
	_shaders.emplace("avc", std::move(defaultShader));

	stbi_set_flip_vertically_on_load(true);

	/*int width, height, numChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels);

	if (data == nullptr || width == 0 || height == 0 || numChannels == 0)
	{
		std::cerr << "Failed to open image " << path << std::endl;
		GL3::StackTrace::PrintStack();
		return false;
	}

	stbi_image_free(data);*/

	_blockSize = 1.0f;
	_blockThreshold = 7;
	_gridLength = configure["grid"].as<size_t>();
	_gridBlockCounts = _gridLength * _gridLength * _gridLength;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribute(1, 16);

	std::vector<glm::ivec4> gridBlocks(_gridBlockCounts);
#pragma omp parallel for
	for (size_t i = 0; i < _gridBlockCounts; ++i)
		gridBlocks[i] = glm::ivec4(i % _gridLength, (i / _gridLength) % _gridLength, i / (_gridLength * _gridLength), distribute(gen))
						- glm::ivec4(_gridLength * 0.5f, _gridLength * 0.5f, _gridLength * 0.5f, 0);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::ivec4) * gridBlocks.size(), gridBlocks.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(0, 4, GL_INT, sizeof(glm::ivec4), (void*)0);
	glBindVertexArray(0);

	std::vector<char> data;
	GL3::AssetLoader::LoadRawFile(RESOURCES_DIR "/textures/terrain.512.2048.rgba.ub.raw", data);

	glGenTextures(1, &_terrain);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _terrain);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, 512, 512, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

void AVCBlockWorld::OnCleanUp()
{
	if (_vbo) glDeleteBuffers(1, &_vbo);
	if (_vao) glDeleteVertexArrays(1, &_vao);
}

void AVCBlockWorld::OnUpdate(double dt)
{
	(void)dt;
}

void AVCBlockWorld::OnDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.8f, 1.0f);

	auto& avcShader = _shaders["avc"];
	avcShader->BindShaderProgram();
	avcShader->SendUniformVariable("blockSize", _blockSize);
	avcShader->SendUniformVariable("blockThreshold", _blockThreshold);
	avcShader->SendUniformVariable("terrain", 0);
	_cameras[0]->BindCamera(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _terrain);
	glBindVertexArray(_vao);
	glDrawArrays(GL_POINTS, 0, _gridBlockCounts);
	glBindVertexArray(0);
}

void AVCBlockWorld::OnProcessInput(unsigned int key)
{
	if (key == GLFW_KEY_LEFT_BRACKET)
		_blockThreshold = _blockThreshold > 0 ? _blockThreshold - 1 : 0;
	if (key == GLFW_KEY_RIGHT_BRACKET)
		_blockThreshold = _blockThreshold < 15 ? _blockThreshold + 1 : 15;
}

void AVCBlockWorld::OnProcessResize(int width, int height)
{
	(void)width;
	(void)height;
}