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

Luna::UniquePtr<Luna::Application> Luna::CreateApplication()
{
	return Luna::MakeUnique<Luna::Application>();
}