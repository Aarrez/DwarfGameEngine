#pragma once
#include <iostream>

namespace Engine::Misc{

    template<typename arrType>
    class Misc{
    public:
        template<size_t N>
        static size_t ArraySize(arrType (&arr)[N]);
    };




}