#include <iostream>

#include "DwarfEngine/DwarfEngine.h"


int main() {

    Dwarf::DwarfEngine engine{};
    engine.Init();
    while(!glfwWindowShouldClose(engine.window)) {
        engine.Update();
        engine.Render();
    }
    engine.Shutdown();

    return 0;
}

