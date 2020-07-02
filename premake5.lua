workspace "Engine"
    architecture "x64"
    startproject "PuzzleGame"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder (solution directory) ---
IncludeDir = {}
IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"
IncludeDir["GLAD"] = "Engine/vendor/GLAD/include"
IncludeDir["GLM"] = "Engine/vendor/GLM"

group "Dependencies"
    include "Engine/vendor/GLFW"
    include "Engine/vendor/GLAD"

group ""

projectheaderdir = "%{prj.name}/src/**.h"
projectcppdir = "%{prj.name}/src/**.cpp"

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "Epch.h"
    pchsource "Engine/src/Epch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Engine/vendor/spdlog/include",
        "Engine/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}"
    }

    links {
        "GLFW",
        "GLAD",
        "opengl32.lib"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "ENG_PLATFORM_WINDOWS",
            "ENG_BUILD_DLL"
        }

    filter "configurations:Debug"
        defines "ENG_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "ENG_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "ENG_DIST"
        runtime "Release"
        optimize "on"

project "PuzzleGame"
    location "PuzzleGame"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    links {
        "Engine"
    }

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "Engine/src",
        "Engine/vendor/spdlog/include",
        "%{IncludeDir.GLM}"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "ENG_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "ENG_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "ENG_RELEASE"
        optimize "on"

    filter "configurations:Dist"
        defines "ENG_DIST"
        optimize "on"
