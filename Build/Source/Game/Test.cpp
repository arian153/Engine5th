//
#include <iostream>
#include "../System/Math/Math.hpp"
#include "../System/Core/Core.hpp"
#include "../System/Core/Utility/ConsoleUtility.hpp"

int WINAPI WinMain(HINSTANCE /*instance*/, HINSTANCE /*prev_instance*/, LPSTR /*cmd_line*/, int /*show_cmd*/)
{
    E5_LEAK_CHECKS(-1);

    Engine5::Console::CreateConsole();
    auto app = new Engine5::Application();
    app->Run();
    delete app;
    Engine5::Console::DestroyConsole();
    return 0;
}
