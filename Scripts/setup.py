import os
import subprocess
import platform

from SetupPython import PythonConfiguration as PythonRequirements

# Make sure everything we need for the setup is installed
PythonRequirements.Validate()

from SetupPremake import PremakeConfiguration as PremakeRequirements
#from SetupVulkan import VulkanConfiguration as VulkanRequirements # Might re-enable this later if I want to support vulkan at some point
os.chdir('./../')

premakeInstalled = PremakeRequirements.Validate()
#VulkanRequirements.Validate()

if (premakeInstalled):
    if platform.system() == "Windows":
        print("\nRunning premake...")
        subprocess.call([os.path.abspath("Win-GenerateProjects.bat"), "nopause"])

    print("\nSetup completed!")
else:
    print("Luna requires Premake to generate project files.")