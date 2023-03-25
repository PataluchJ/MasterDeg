#include "List.hpp"
#include "Distribution.hpp"

#include <iostream>

template<typename T>
void printList(std::list<T>& list){
    for(auto it = list.begin(); it != list.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << "\n";
}

int main(){
    try{
        std::shared_ptr<VariableProbability> vp = std::make_shared<UniformProbability>(UniformProbability());
        DiscreteDistribution dd(vp);

        std::cout << dd.getRandom() << "\n";
    
    }
    catch(std::exception& e){
        std::cout << "Error: " << e.what() << "\n";
    }
    return 0;
}