#include "ConcreateHashFunction.hpp"

#include <climits>
#include <iostream>

double calcAvg(std::vector<double>& vec){
    double avg = 0.0;
    double div = vec.size();
    for(auto v : vec){
        avg += v / div;
    }
    return avg;
}


void run(){
    //auto hash = StandardHash();
    auto hash = BiasedHash(3, [](Hash a, Hash b){return a > b;});
    size_t testSize = UINT32_MAX - 1;
    auto gen = RangeBasedSetGenerator(0, testSize);

    double tempAccu = 0.0;
    std::vector<double> packs;
    size_t packSize = 50000;
    size_t r = 0;
    double div = packSize;
    
    while(gen.remaining() > 0){
        tempAccu += (double)(hash(gen.next()))/div;
        r++;
        if(r % packSize == 0 && r > 0){
            packs.push_back(tempAccu);
            std::cout << "R: " << gen.remaining() << " " << calcAvg(packs) << "\n";
            tempAccu = 0.0;
        } 
    }

    std::cout << "Result: " << calcAvg(packs) << "\n";
}

int main(){
    run();
}