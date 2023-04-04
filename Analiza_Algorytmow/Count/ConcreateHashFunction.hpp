#pragma once

#include "HashFunction.hpp"

#include <random>
#include <functional>

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

class ShorterHash : public HashFunction {
    public:
    ShorterHash(unsigned char bitsToRemove, SetElement offset = 0) 
        : bitsToRemove(bitsToRemove), offset(offset) {}
    Hash operator()(SetElement element){
        gen.seed(element + offset);
        auto hs = gen();
        hs >>= bitsToRemove;
        hs <<= bitsToRemove;
        return (double)(hs)/(double)(gen.max());
    }

    private:
    std::mt19937 gen;
    const unsigned char bitsToRemove;
    const SetElement offset;
};

class BiasedHash : public HashFunction {
    public:
    BiasedHash(unsigned char steps, std::function<bool(Hash, Hash)> comp, SetElement offset = 0) 
        : steps(steps), comp(comp), offset(offset) {}
    Hash operator()(SetElement element){
        gen.seed(element + offset);
        auto hs = gen();
        for(unsigned char s = 0; s < steps; s++){
            auto nhs = gen();
            if(comp(hs,nhs))
                hs = nhs;
        }
        return (double)(hs)/(double)(gen.max());
    }

    private:
    std::mt19937 gen;
    const unsigned char steps;
    const std::function<bool(Hash, Hash)> comp;
    const SetElement offset;
};