#include <Luna.h>

#include "SandboxLayer.h"

class Sandbox : public Luna::Application
{
public:
	Sandbox()
	{
		PushLayer(new SandboxLayer());
	}

	~Sandbox()
	{
		
	}
};

Luna::UniquePtr<Luna::Application> Luna::CreateApplication()
{
	return Luna::MakeUnique<Sandbox>();
}