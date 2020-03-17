workspace "Project-E5"
    configurations {"Debug", "Release"}
    platforms {"Win64"}
    warnings "Extra"
    location "Build"
    

project "Engine5"
    kind "WindowedApp"
    language "C++"
    location "Build"
    targetname ("Engine5")
    targetdir "Bin/%{cfg.buildcfg}-X64"
    objdir "Build/Obj/%{cfg.buildcfg}-X64"
    systemversion "10.0"
    flags {"MultiProcessorCompile", "NoMinimalRebuild", "NoBufferSecurityCheck"}

    includedirs {"Include"}
    libdirs {"Lib"}
    

    files {"**.h", "**.hpp", "**.cpp", "**.inl"}
    removefiles {"**Template.cpp"}

    filter "platforms:Win64"
        system "Windows"
        architecture "x64"
        defines {"_CRT_SECURE_NO_WARNINGS", "E5_WIN32", "E5_DIRECTX11"}
        buildoptions {"-std:c++17"}
        links {"dxgi", "d3d11", "d3dcompiler", "d2d1", "dwrite", "dsound", "dxguid", "winmm", "Xinput9_1_0"}
        disablewarnings {"4505", "4456", "4201", "4100", "4189", "4458", "4819", "4127", "4701", "4291", "6385"}

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
        debugdir "Bin/Debug-X64"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
        debugdir "Bin/Release-X64"

