#pragma once

#ifdef LU_PLATFORM_WINDOWS

extern Luna::UniquePtr<Luna::Application> Luna::CreateApplication();

int main(int argc, char** argv)
{
	Luna::Log::Init();
	LU_CORE_INFO("Logger Initialized!");
	LU_INFO("Client Logger Initialized!")

	auto App = Luna::CreateApplication();
	App->Run();
}
#endif // LU_PLATFORM_WINDOWS