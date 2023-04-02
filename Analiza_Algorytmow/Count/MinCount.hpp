#include "Set.hpp"
#include "HashFunction.hpp"

#include <memory>
#include <vector>

class MinCount {
    public:
    MinCount(std::shared_ptr<HashFunction> hash, size_t arraySize);
    
    void in(SetElement v);
    size_t getEstimation();
    
    std::vector<double> table;
    
    private:
    
    bool validForInsert(double h);
    bool contains(double h);
    void insert(double h);
    size_t countNotOne();

    
    std::shared_ptr<HashFunction> hash;
};