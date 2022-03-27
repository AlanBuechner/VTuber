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
--IncludeDir["ImGui"] = "Vtuber/vendor/imgui"
IncludeDir["glm"] = "Vtuber/vendor/Glm"
IncludeDir["stb_image"] = "Vtuber/vendor/Stb_Image"
IncludeDir["assimp"] = "Vtuber/vendor/assimp"
IncludeDir["entt"] = "Vtuber/vendor/entt/include"
--IncludeDir["yaml"] = "Vtuber/vendor/yaml-cpp/include"
--IncludeDir["ImGuizmo"] = "Vtuber/vendor/ImGuizmo"

--include "Vtuber/vendor/imgui"
--include "Vtuber/vendor/yaml-cpp"

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
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/Glm/glm/**.hpp",
		"%{prj.name}/vendor/Glm/glm/**.inl",
--		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
--		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
		"%{prj.name}/Assets/Shaders/**.hlsl"
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
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.assimp}/include",
		"%{IncludeDir.entt}",
--		"%{IncludeDir.yaml}",
--		"%{IncludeDir.ImGuizmo}"
	}

	links 
	{ 
--		"ImGui",
--		"Yaml",
		"%{prj.location}/vendor/assimp/assimp-vc140-mt.lib",
		"dxguid.lib"
	}

	-- TODO : fix environment variables. need to set manually
	environmentVariables =
	{
		"PATH=%PATH%;$(ProjectDir)vendor\assimp"
	}

	filter { "files:**.hlsl" }
		shadermodel "5.0"
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

--include "Vtuber/vendor/assimp"