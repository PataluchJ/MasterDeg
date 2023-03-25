#pragma once
#include <cstddef>
#include <cstdint>
#include <ostream>

using Variable = uint32_t;
using Cost = uint32_t;
using Range = uint32_t;

struct CountableVariable{
    CountableVariable(Variable v);

    Variable v;
    size_t count;

    friend std::ostream& operator<<(std::ostream& os, const CountableVariable& dt);
};
