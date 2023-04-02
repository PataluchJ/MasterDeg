#include "ConcreateHashFunction.hpp"
#include "MinCount.hpp"
#include "Set.hpp"

#include <iostream>
bool withingLimits(RangeBasedSetGenerator& gen, MinCount& min){
    while(gen.remaining() > 0)
        min.in(gen.next());
    double ratio = (double)(gen.size()) / (double)(min.getEstimation());
    return fabs(1 - ratio) < 0.1;
}

double test(size_t k){
    size_t rangeStart = 1;
    size_t rangeEnd = 1;

    size_t successfull = 0;
    auto hf = std::shared_ptr<HashFunction>(new StandardHash());
    size_t step = 4;
    for(size_t n = 1; n <= 10000; n += step){
        rangeStart = rangeEnd;
        rangeEnd += n;
        MinCount mc(hf, k);
        RangeBasedSetGenerator gen(rangeStart, rangeEnd);
        if(withingLimits(gen, mc))
            successfull++;
    }
    return (double)(successfull) / (10000.0/((double)step));

}
int main(){

    size_t leftK = 2;
    size_t rightK = 1000;
    size_t k;
    double amountLesserThan10 = 0.0;
    double epsilon = 0.000001;
    do
    {
        k = (rightK + leftK) / 2;  
        amountLesserThan10 = test(k);
        std::cout << k << " : " << amountLesserThan10 << " " << fabs(0.95 - amountLesserThan10) << "\n";
        if(amountLesserThan10 < 0.95)
            leftK = k;
        else
            rightK = k;

        if(leftK >= rightK){
            std::cout << "Not founded withing epsilon.\n";
            break;
        }
    } while (fabs(0.95 - amountLesserThan10) > epsilon);
    
    std::cout << "Optimal k = " << k << " with p = " << amountLesserThan10 << "\n";

}