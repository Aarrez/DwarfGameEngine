#include "TestCode.h"

template<typename arrType>
template<size_t N>
size_t Dwarf::Misc::Misc<arrType>::ArraySize(arrType (&arr)[N]) {
    return N;
}
