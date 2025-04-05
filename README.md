# LunaEngine
 
Luna is a custom 3D rendering engine written in C++ using D3D12 (and eventually Vulkan). This project is intended as a way for me to dive deeper into aspects game engine development in the context of my own framework, as well as provide a sandbox to experiment with/learn about graphics programming techniques.

***

## Project Setup
This project has been built using Visual Studio 2022, and currently only runs on Windows. I eventually want to make this cross-platform in the future.

<ins>**1. Downloading the Repository:**</ins>

Clone the repository with `git clone --recurse-submodules https://github.com/Epics123/LunaEngine.git`

<ins>**2. Dependencies:**</ins>

1. This project uses git submodules for some third party libraries, and you should have them if you followed step 1 correctly. If you want to be extra sure, run `git submodule update --init --recursive`.
2. Run the `Setup.bat` script located in the `Scripts` folder. This will download all the necesary prerequisites for the project.
3. After all the prerequisites have been installed, a `.sln` file for the engine should be generated automatically.
	- If changes are made to `premake5.lua` or the solution needs to be regenerated, run the `Win-GenerateProjects.bat` script located in the main project directory.

## Running the Project
* Open `LunaEngine/LunaEngine.sln`
* Select configuration: Debug, Release, or Dist
* Select platform (currently only supports Windows x64)
* Set `Sandbox` as the startup project if it is not already
* Build and run

## Controls
* TODO: Add controls when implemented
