#pragma once

#include "Types.hpp"

#include <random>
#include <vector>
#include <memory>

class VariableProbability {
    public:
    VariableProbability(Range range);
    virtual ~VariableProbability() = default;

    virtual double propability(Variable x) = 0;
    Range getRange();

    protected:
    Range range;
};

class DiscreteDistribution{
    public:
    DiscreteDistribution(std::shared_ptr<VariableProbability> probability);
    ~DiscreteDistribution() = default;

    Range getRange();
    Variable getRandom();

    private:
    Range range;
    std::vector<double> variableProbabilityCache;

    std::random_device rd{};
    std::shared_ptr<std::mt19937> gen{nullptr};
    std::shared_ptr<std::uniform_real_distribution<>> dis{nullptr};

};

class UniformProbability : public VariableProbability {
    public:
    UniformProbability(Range range = 100);
    ~UniformProbability();

    double propability(Variable x) override;

    private:
    Range range;
    double p;
};

class HarmonicProbability : public VariableProbability {
    public:
    HarmonicProbability(Range range = 100);
    ~HarmonicProbability();

    double propability(Variable x) override;

    private:
    double harmonic{0.0};
};

class SecondHarmonicProbability : public VariableProbability {
    public:
    SecondHarmonicProbability(Range range = 100);
    ~SecondHarmonicProbability();

    double propability(Variable x) override;

    private:
    double harmonic{0.0};
};

class GeometricProbability : public VariableProbability {
    public:
    GeometricProbability(Range range = 100);
    ~GeometricProbability();

    double propability(Variable x) override;

    private:
};