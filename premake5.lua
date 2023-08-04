-- Workspace
workspace "Water-Simulation"
architecture "x64"
configurations "Release"

-- Include projects
include "Dependencies/GLFW"

-- Main project
project "Water-Simulation"
location "Water-Simulation"
kind "ConsoleApp"
language "C++"
cppdialect "C++17"
targetdir "bin/%{prj.name}"
objdir "obj/%{prj.name}"
files {"%{prj.name}/src/**.cpp", "%{prj.name}/src/**.c", "%{prj.name}/src/**.h"}
includedirs {"Dependencies/Glad/include", "Dependencies/GLFW/include", "Dependencies/GLM/include"}
links {"GLFW"}

-- Because only building release optimize is on
optimize "On"

-- For windows systems
filter "system:windows"
systemversion "latest"
links {"opengl32.lib"}
