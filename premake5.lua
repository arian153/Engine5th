workspace "Project-E5"
    configurations {"Debug", "Release"}
    warnings "Extra"
    location ""
    platforms { "x64" }
    filter { "platforms:x64" }
        system "Windows"
        architecture "x64"
    filter { }

project "Engine5"
    kind "WindowedApp"
    language "C++"
    location ""
    targetname ("Engine5")
    targetdir "Bin/%{cfg.buildcfg}"
    objdir "Obj/%{cfg.buildcfg}"
    debugdir "Bin/%{cfg.buildcfg}"
    systemversion "10.0"
    flags {"MultiProcessorCompile", "NoMinimalRebuild", "NoBufferSecurityCheck"}
    warnings "Extra"
    includedirs {"Include"}
    libdirs {"Lib"}
    

    files {"**.h", "**.hpp", "**.cpp", "**.inl"}
    removefiles {"**Template.cpp"}

    filter "platforms:x64"
        system "Windows"
        architecture "x64"
        defines {"_CRT_SECURE_NO_WARNINGS", "E5_WIN32", "E5_DIRECTX11"}
        buildoptions {"-std:c++17"}
        links {"dxgi", "d3d11", "d3dcompiler", "d2d1", "dwrite", "dsound", "dxguid", "winmm", "Xinput9_1_0"}
        disablewarnings {"4505", "4456", "4201", "4100", "4189", "4458", "4819", "4127", "4701", "4291", "6385"}

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
   

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
     

     

