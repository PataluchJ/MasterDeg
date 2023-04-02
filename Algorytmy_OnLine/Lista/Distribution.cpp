#include "Distribution.hpp"

#include <math.h>
#include <iostream>
#include <climits>
#include <exception>

VariableProbability::VariableProbability(Range range)
    : range(range) {}

Range VariableProbability::getRange() { return range; }

DiscreteDistribution::DiscreteDistribution(std::shared_ptr<VariableProbability> probability)
    : range(probability->getRange())
{
    variableProbabilityCache.resize(range+1, 0.0);
    for(Variable i = 1; i <= range; i++){
        variableProbabilityCache[i] = variableProbabilityCache[i-1] + probability->propability(i);
    }
    //*
    //variableProbabilityCache[range] = 1.0; // I hate floating point error accumulation
    if((fabs(variableProbabilityCache.back() - 1.0) ) > std::numeric_limits<double>::epsilon() * 10.0 )
    {
        std::cout << "Distribution sanity failure! " << variableProbabilityCache.back() <<
            " " << fabs(variableProbabilityCache.back() - 1.0)  << " > " << std::numeric_limits<double>::epsilon() * 10.0 << "\n";

        //throw std::runtime_error("Distribution sanity failure.");
    }
    //*/
    //rd = std::make_shared<std::random_device>(new std::random_device());
    gen = std::make_shared<std::mt19937>(std::mt19937((*rd)()));
    dis = std::make_shared<std::uniform_real_distribution<>>(std::uniform_real_distribution<>(0.0, variableProbabilityCache.back()));
}

Range DiscreteDistribution::getRange(){
    return range;
}

Variable DiscreteDistribution::getRandom(){
    double x = (*dis)(*gen);
    for(Variable v = 1; v <= range; v++){
        if(x - variableProbabilityCache[v] < 0.0)
            return v;
    }
    std::cout << "Random value out of distribution range. Generated " << x << " while last cache is " << variableProbabilityCache.back() << "\n";
    throw std::runtime_error("Random value out of distribution range");
}

UniformProbability::UniformProbability(Range range)
    : VariableProbability(range), p(1.0 / (double)(range)) {}

UniformProbability::~UniformProbability() {}

double UniformProbability::propability(Variable x){
    return p;
}

HarmonicProbability::HarmonicProbability(Range range)
    : VariableProbability(range) {
    for(size_t i = range; i >= 1; i--){
        harmonic += 1.0 / (double)(i);
    }
}
HarmonicProbability::~HarmonicProbability() {}

double HarmonicProbability::propability(Variable x){
    return 1.0 / ( (double)(x) * harmonic);
}

SecondHarmonicProbability::SecondHarmonicProbability(Range range)
    : VariableProbability(range) {
    for(size_t i = range; i >= 1; i--){
        harmonic += 1.0 / (double)(i*i);
    }
}
SecondHarmonicProbability::~SecondHarmonicProbability() {}

double SecondHarmonicProbability::propability(Variable x){
    return 1.0 / ( (double)(x*x) * harmonic);
}

GeometricProbability::GeometricProbability(Range range)
    : VariableProbability(range) {}
    
GeometricProbability::~GeometricProbability() {}

double GeometricProbability::propability(Variable x) {
    if(x == range){
        return 1.0 / (double)(pow(2.0, x-1));
    }

    return 1.0 / (double)(pow(2.0, x));
}