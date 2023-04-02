#include "Set.hpp"

RangeBasedSetGenerator::RangeBasedSetGenerator(SetElement first, SetElement last)
    : first(first), current(first), last(last) {}

SetElement RangeBasedSetGenerator::next(){
    if(current == last)
        reset();
    auto element = current;
    current++;
    return element;
}

unsigned int RangeBasedSetGenerator::remaining(){
    return last - current;
}

unsigned int RangeBasedSetGenerator::size(){
    return last - first;
}

void RangeBasedSetGenerator::reset(){
    current = first;
}