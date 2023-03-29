#pragma once

#include "HashFunction.hpp"

#include <random>

class StandardHash : public HashFunction {
    public:
    StandardHash(SetElement offset = 0) : offset(offset) {}
    Hash operator()(SetElement element){
        gen.seed(element + offset);
        return (double)(gen())/(double)(gen.max());
    }

    private:
    std::mt19937 gen;
    const SetElement offset;
};