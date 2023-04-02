#pragma once
#include "Set.hpp"

using Hash = double;

class HashFunction {
    public:
    virtual Hash operator()(SetElement element) = 0;
};