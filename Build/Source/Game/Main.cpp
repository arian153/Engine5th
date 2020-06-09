#include <iostream>
#include "../System/Core/Core.hpp"
#include "../Manager/Component/ComponentRegistry.hpp"
#include "GameLogic/Factory/ControllerFactory.h"

int WINAPI WinMain(HINSTANCE /*instance*/, HINSTANCE /*prev_instance*/, LPSTR /*cmd_line*/, int /*show_cmd*/)
{
    E5_LEAK_CHECKS(-1);
    auto app = new Engine5::Application();
    app->Initialize();
    //Add game components
    app->GetComponentRegistry()->AddFactory(new Game::ControllerFactory);

    app->Update();
    app->Shutdown();
    delete app;
    return 0;
}