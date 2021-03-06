#ifndef AVC_BLOCKWORLD_HPP
#define AVC_BLOCKWORLD_HPP

#include <GL3/Application.hpp>

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

private:
};

#endif //! end of AVCBlockWorld.hpp