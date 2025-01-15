#include "TestCode.h"

template<typename arrType>
template<size_t N>
size_t Engine::Misc::Misc<arrType>::ArraySize(arrType (&arr)[N]) {
    return N;
}
