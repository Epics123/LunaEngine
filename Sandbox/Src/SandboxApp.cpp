#include <Luna.h>

class Sandbox : public Luna::Application
{
public:
	Sandbox()
	{
		
	}

	~Sandbox()
	{
		
	}
};

Luna::Application* Luna::CreateApplication()
{
	return new Sandbox();
}