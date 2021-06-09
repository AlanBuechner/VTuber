workspace "Vtuber"
	architecture "x64"
	startproject "Vtuber"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
--IncludeDir["ImGui"] = "GameEngine/vendor/imgui"
IncludeDir["glm"] = "Vtuber/vendor/Glm"
--IncludeDir["stb_image"] = "GameEngine/vendor/stb_image"
--IncludeDir["entt"] = "GameEngine/vendor/entt/include"
--IncludeDir["yaml"] = "GameEngine/vendor/yaml-cpp/include"
--IncludeDir["ImGuizmo"] = "GameEngine/vendor/ImGuizmo"

--include "GameEngine/vendor/imgui"
--include "GameEngine/vendor/yaml-cpp"

project "Vtuber"
	location "Vtuber"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

--	pchheader "pch.h"
--	pchsource "GameEngine/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
--		"%{prj.name}/vendor/stb_image/**.h",
--		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/Glm/glm/**.hpp",
		"%{prj.name}/vendor/Glm/glm/**.inl",
--		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
--		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
--		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
--		"%{IncludeDir.stb_image}",
--		"%{IncludeDir.entt}",
--		"%{IncludeDir.yaml}",
--		"%{IncludeDir.ImGuizmo}"
	}

	links 
	{ 
--		"ImGui",
--		"Yaml"
		"dxguid.lib"
	}

	-- need to fix shader propertys

	filter { "files:**.hlsl" }
		shadermodel "4.0"
		shaderobjectfileoutput("ShaderBin/".."%{file.basename}"..".cso")

	filter { "files:**.pixel.hlsl" }
		shadertype "Pixel"

	filter { "files:**.vertex.hlsl" }
		shadertype "Vertex"

--	filter "files:vendor/ImGuizmo/**.cpp"
--	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			
		}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"
