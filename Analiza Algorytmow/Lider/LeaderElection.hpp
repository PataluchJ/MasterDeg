#pragma once 

#include <cstddef>

class NodeSet {
    public:
    NodeSet(size_t n);

    enum class SlotState {
        Null,
        Single,
        Noise
    };

    SlotState simulateSlotState(double probability);

    private:
    size_t n;
};

class Algorithm {
    public:
    virtual size_t simulate(NodeSet& nodes) = 0;
};


class SetAmount : public Algorithm {
    public:
    SetAmount(size_t n);
    size_t simulate(NodeSet& nodes) override;

    private:
    const double probability;
};

class UpperBound : public Algorithm {
    public:
    UpperBound(size_t u);
    size_t simulate(NodeSet& nodes) override;
    bool oneRound(NodeSet& nodes);
    private:
    size_t slotsPerRound;
};
