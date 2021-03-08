#ifndef AVC_BLOCKWORLD_HPP
#define AVC_BLOCKWORLD_HPP

#include <GL3/Application.hpp>
#include <GL3/GLTypes.hpp>

class AVCBlockWorld : public GL3::Application
{
public:
	//! Default constructor
	AVCBlockWorld();
	//! Default desctrutor
	~AVCBlockWorld();
	//! Returns overrided Application title
	const char* GetAppTitle() const override
	{
		return "Sample Application";
	}
protected:
	bool OnInitialize(std::shared_ptr<GL3::Window> window, const cxxopts::ParseResult& configure) override;
	void OnCleanUp() override;
	void OnUpdate(double dt) override;
	void OnDraw() override;
	void OnProcessInput(unsigned int key) override;
	void OnProcessResize(int width, int height) override;

private:
	GLuint _vao, _vbo, _terrain;
	float _blockSize;
	int _blockThreshold;
	size_t _gridLength;
	size_t _gridBlockCounts;
};

#endif //! end of AVCBlockWorld.hpp