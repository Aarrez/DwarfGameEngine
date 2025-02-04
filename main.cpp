#include <iostream>

#include "Engine/GameEngine.h"


int main() {

    Engine::GameEngine engine{};

    engine.Allocate();
    engine.InitializeGL();
    engine.Init();
    while(!glfwWindowShouldClose(engine.window)) {
        engine.Update();
        engine.Render();
    }
    engine.Shutdown();

    return 0;
}

