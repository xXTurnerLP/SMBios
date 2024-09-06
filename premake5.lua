workspace "SMBios"
	configurations {
		"Debug",
		"Release"
	}

	architecture "x64"

	src_dir = "src/"

	project "SMBios_LIB"
		location "build/"
		kind "StaticLib"
		language "C++"
		cppdialect "C++20"
		buildoptions { "/Zc:preprocessor", "/utf-8" } -- preprocessor conformance mode

		targetdir "build/bin/%{prj.name}/%{cfg.buildcfg}"
		objdir "build/bin/obj/%{prj.name}/%{cfg.buildcfg}"
		debugdir "$(TargetDir)" -- set debug working dir to the directory where the binary is

		files {
			src_dir .. "**.hpp",
			src_dir .. "**.cpp",
		}

		includedirs {
			src_dir,
		}

		links {

		}

		filter "configurations:Debug"
			defines { "DEBUG" }

			symbols "on"
			optimize "off"
	
		filter "configurations:Release"
			defines { "RELEASE" }
			
			symbols "off"
			optimize "speed"
			inlining "auto"
			flags { "LinkTimeOptimization" }

	project "SMBios_DLL"
		location "build/"
		kind "SharedLib"
		language "C++"
		cppdialect "C++20"
		buildoptions { "/Zc:preprocessor", "/utf-8" } -- preprocessor conformance mode

		targetdir "build/bin/%{prj.name}/%{cfg.buildcfg}"
		objdir "build/bin/obj/%{prj.name}/%{cfg.buildcfg}"
		debugdir "$(TargetDir)" -- set debug working dir to the directory where the binary is

		files {
			src_dir .. "**.hpp",
			src_dir .. "**.cpp",
		}

		includedirs {
			src_dir,
		}

		links {

		}

		filter "configurations:Debug"
			defines { "DEBUG" }

			symbols "on"
			optimize "off"
	
		filter "configurations:Release"
			defines { "RELEASE" }
			
			symbols "off"
			optimize "speed"
			inlining "auto"
			flags { "LinkTimeOptimization" }

	project "Example"
		location "build/"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++20"
		buildoptions { "/Zc:preprocessor", "/utf-8" } -- preprocessor conformance mode

		targetdir "build/bin/%{prj.name}/%{cfg.buildcfg}"
		objdir "build/bin/obj/%{prj.name}/%{cfg.buildcfg}"
		debugdir "$(TargetDir)" -- set debug working dir to the directory where the binary is

		files {
			src_dir .. "**.hpp",
			src_dir .. "**.cpp",
			"example/test.cpp",
		}

		includedirs {
			src_dir,
		}

		links {

		}

		filter "configurations:Debug"
			defines { "DEBUG" }

			symbols "on"
			optimize "off"
	
		filter "configurations:Release"
			defines { "RELEASE" }
			
			symbols "off"
			optimize "speed"
			inlining "auto"
			flags { "LinkTimeOptimization" }
