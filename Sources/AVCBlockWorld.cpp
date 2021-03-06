#include <AVCBlockWorld.hpp>
#include <GL3/Window.hpp>
#include <GL3/PerspectiveCamera.hpp>
#include <GL3/Shader.hpp>
#include <GL3/Texture.hpp>
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
	_shaders.emplace("default", std::move(defaultShader));

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

	_blockSize = 0.3f;
	_gridLength = configure["grid"].as<size_t>();
	_gridBlockCounts = _gridLength * _gridLength * _gridLength;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribute(1, 4);

	std::vector<glm::ivec4> gridBlocks(_gridBlockCounts);
#pragma omp parallel for
	for (size_t i = 0; i < _gridBlockCounts; ++i)
		gridBlocks[i] = glm::ivec4(i % _gridLength, (i / _gridLength) * _gridLength, i / (_gridLength * _gridLength), distribute(gen));

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::ivec4) * gridBlocks.size(), gridBlocks.data(), GL_STATIC_USAGE);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_INT, GL_FALSE, sizeof(glm::ivec4), (void*)0);
	glBindVertexArray(0);

	return true;
}

void AVCBlockWorld::OnCleanUp()
{
	//! Do nothing
}

void AVCBlockWorld::OnUpdate(double dt)
{
	(void)dt;
}

void AVCBlockWorld::OnDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.8f, 1.0f);
}

void AVCBlockWorld::OnProcessInput(unsigned int key)
{
	(void)key;
}