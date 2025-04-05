#pragma once

#ifdef LU_PLATFORM_WINDOWS

extern Luna::Application* Luna::CreateApplication();

int main(int argc, char** argv)
{
	auto App = Luna::CreateApplication();
	App->Run();
	delete App;
}
#endif // LU_PLATFORM_WINDOWS