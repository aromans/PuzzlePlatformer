workspace "Engine"
    architecture "x86_64"
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
IncludeDir["GLM"] = "Engine/vendor/GLM/glm"

group "Dependencies"
    include "Engine/vendor/GLFW"

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

    --pchheader "enpch.h"
    --pchsource "Engine/src/enpch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/GLM/glm/**.hpp",
        "%{prj.name}/vendor/GLM/glm/**.inl"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLM}"
    }

    links {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"
    
        defines {
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

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Engine/vendor/spdlog/include",
        "Engine/src",
        "Engine/vendor",
        "%{IncludeDir.GLM}"
    }

    links {
        "Engine"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "ENG_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "ENG_RELEASE"
        optimize "on"

    filter "configurations:Dist"
        defines "ENG_DIST"
        optimize "on"
