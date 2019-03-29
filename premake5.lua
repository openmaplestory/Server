workspace "OpenMS"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "OpenMSWindows"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "OpenMS"
	location "OpenMS"
	kind "SharedLib"
	language "C++"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "omspch.h"
	pchsource "OpenMS/src/omspch.cpp"
	
	files
	{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"OpenMS/vendor/spdlog/include"
	}

	filter "system:Windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"OMS_PLATFORM_WINDOWS",
			"OMS_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/OpenMSWindows\"")
		}
	
	filter "system:Unix"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"OMS_PLATFORM_LINUX",
			"OMS_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/OpenMSLinux\"")
		}

	filter "configurations:Debug"
		defines "OMS_DEBUG"
		runtime "Debug"
		symbols "On"

		filter "configurations:Release"
		defines "OMS_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "OMS_DIST"
		runtime "Release"
		optimize "On"

project "OpenMSWindows"
	location "OpenMSWindows"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"OpenMS/src",
		"OpenMS/vendor/spdlog/include"
	}

	links
	{
		"OpenMS"
	}

	filter "system:Windows"
		defines
		{
			"OMS_PLATFORM_WINDOWS"
		}

		cppdialect "C++17"
		systemversion "latest"

		filter "configurations:Debug"
			defines "NGC_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "NGC_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "NGC_DIST"
			runtime "Release"
			optimize "On"

project "OpenMSLinux"
	location "OpenMSLinux"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"OpenMS/src",
		"{IncludeDir.spdlog}"
	}

	links
	{
		"OpenMS"
	}

	filter "system:Unix"
		defines
		{
			"OMS_PLATFORM_LINUX"
		}

		cppdialect "C++17"
		systemversion "latest"

		filter "configurations:Debug"
			defines "NGC_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "NGC_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "NGC_DIST"
			runtime "Release"
			optimize "On"