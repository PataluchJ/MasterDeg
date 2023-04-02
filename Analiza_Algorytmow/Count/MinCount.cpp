#include "MinCount.hpp"

#include <algorithm>

MinCount::MinCount(std::shared_ptr<HashFunction> hash, size_t arraySize) : hash(hash) {
    table.resize(arraySize, 1.0);
}

void MinCount::in(SetElement v){
    auto h = (*hash)(v);
    if(validForInsert(h))
        insert(h);
}

size_t MinCount::getEstimation(){
    if(table.back() == 1.0)
        return countNotOne();
    
    return (double)(table.size() - 1) / table.back();
}

bool MinCount::validForInsert(double h){
    if(table.back() < h)
        return false;
    return !contains(h);
}

bool MinCount::contains(double h){
    for(auto existing : table){
        if(existing > h)
            return false;
        if(existing == h)
            return true;
    }
    return false;
}

void MinCount::insert(double h){
    size_t idx = table.size()-1;
    table[idx] = h;
    while(idx > 0 && (table[idx] < table[idx-1])){
        std::swap(table[idx], table[idx-1]);
        idx--;
    }
        
}

size_t MinCount::countNotOne(){
    size_t idx = 0;
    while(idx < table.size() && table[idx] != 1.0)
        idx++;


    return idx;
}