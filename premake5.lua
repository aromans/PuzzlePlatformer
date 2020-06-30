workspace "Engine"
    architecture "X64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder (solution directory) ---
IncludeDir = {}
--IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"
--IncludeDir["GLEW"] = "Engine/vendor/GLEW/include"
--IncludeDir["glm"] = "Engine/vendor/glm"

group "Dependencies"
    --include "Engine/vendor/GLFW"
    --include "Engine/vendor/Glad"
    --include "Engine/vendor/imgui"

group ""

projectheaderdir = "%{prj.name}/src/**.h"
projectcppdir = "%{prj.name}/src/**.cpp"

project "Engine"
    location "Engine"
    kind "SharedLib"    --kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    --pchheader "enpch.h"
    --pchsource "Engine/src/enpch.cpp"

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
        --"%{prj.name}/vendor/glm/glm/**.hpp",
        --"%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs {
        "Engine/vendor/spdlog/include",
        "Engine/src"
        --"%{IncludeDir.GLFW}",
        --"%{IncludeDir.Glad}",
        --"%{IncludeDir.ImGui}",
        --"%{IncludeDir.glm}"
    }

    --links {
    --    "GLFW",
    --    "Glad",
    --    "ImGui",
    --    "opengl32.lib"
    --}

    filter "system:windows"
        systemversion "latest"
    
        defines {
            "ENG_PLATFORM_WINDOWS",
            "ENG_BUILD_DLL"
            --"GLFW_INCLUDE_NONE"
        }

        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/PuzzleGame")
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
        "Engine/vendor"
        --"%{IncludeDir.glm}"
    }

    links {
        "Engine"
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
