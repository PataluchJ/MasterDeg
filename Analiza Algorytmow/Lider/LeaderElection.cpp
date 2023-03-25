#include "LeaderElection.hpp"

#include <math.h>
#include <random>

NodeSet::NodeSet(size_t n)
    : n(n) {}

NodeSet::SlotState NodeSet::simulateSlotState(double probability){
    //double r = ((double) rand() / (RAND_MAX));

    bool transmiting = true;
    for(size_t i = 0; i < n; i++){
        if(((double) rand() / (RAND_MAX)) <= probability){
            if(transmiting)
                return SlotState::Noise;
            transmiting = true;
        }
    }

    if(transmiting)
        return SlotState::Single;
    return SlotState::Null;
}

SetAmount::SetAmount(size_t n)
    : probability(1.0 / (double)(n)) {}

size_t SetAmount::simulate(NodeSet& nodes) {
    size_t currentSlot = 0;
    auto slotState = NodeSet::SlotState::Null;

    while(slotState != NodeSet::SlotState::Single){
        currentSlot++;
        slotState = nodes.simulateSlotState(probability);
    }

    return currentSlot;
}

UpperBound::UpperBound(size_t u)
    : slotsPerRound(round(log2(u) + 0.5)) {}

size_t UpperBound::simulate(NodeSet& nodes) {
    size_t currentSlot = 0;
    auto slotState = NodeSet::SlotState::Null;
    size_t i = 1;
    double probability = 0.5;
    while(slotState != NodeSet::SlotState::Single){
        currentSlot++;
        slotState = nodes.simulateSlotState(probability);

        i++;    
        if(i > slotsPerRound){
            i = 1;
            probability = 0.5;
        }
        else
            probability /= 2;
    }

    return currentSlot;
}

bool UpperBound::oneRound(NodeSet& nodes){
    size_t currentSlot = 0;
    auto slotState = NodeSet::SlotState::Null;
    size_t i = 1;
    double probability = 0.5;
    while(slotState != NodeSet::SlotState::Single && i <= slotsPerRound){
        currentSlot++;
        slotState = nodes.simulateSlotState(probability);
        i++;
        probability /= 2;
    }

    return slotState == NodeSet::SlotState::Single;
}