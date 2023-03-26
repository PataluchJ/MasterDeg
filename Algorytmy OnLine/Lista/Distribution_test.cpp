#include "Distribution.hpp"

#include <thread>
#include <iostream>

constexpr size_t testSize = 50000;

void generateDistribution(DiscreteDistribution& d, std::vector<size_t>& results){
    results.resize(d.getRange()+1, 0);

    for(int i = 0 ; i < testSize; i++){
        try{
            (results)[d.getRandom()]++;
        }
        catch(std::runtime_error& e){
            // UPS
        }
    }
}

void printResult(std::vector<size_t> results){
    for(size_t i = 0; i < results.size(); i++){
        std::cout << i << ": " << (double)results[i]/(double)testSize << "\n";
    }
}


int main(){
    Range range = 100;

    DiscreteDistribution uniformDist(std::make_shared<UniformProbability>(UniformProbability(range)));
    DiscreteDistribution harmonicDist(std::make_shared<HarmonicProbability>(HarmonicProbability(range)));
    DiscreteDistribution secHarmonicDist(std::make_shared<SecondHarmonicProbability>(SecondHarmonicProbability(range)));
    DiscreteDistribution geoDist(std::make_shared<GeometricProbability>(GeometricProbability(range)));  

    std::vector<size_t> uniformResults, harmonicResult,secHarmonicResult, geoResult;
    
    std::cout << "Staring threads...\n";

    std::thread uniform(generateDistribution, std::ref(uniformDist), std::ref(uniformResults));
    std::thread harmonic(generateDistribution, std::ref(harmonicDist), std::ref(harmonicResult));
    std::thread secHarmonic(generateDistribution, std::ref(secHarmonicDist), std::ref(secHarmonicResult));
    std::thread geo(generateDistribution, std::ref(geoDist), std::ref(geoResult));
    
    uniform.join();
    std::cout << "Uniform:\n"; printResult((uniformResults));
    harmonic.join();
    std::cout << "Harmonic:\n"; printResult((harmonicResult));
    secHarmonic.join();
    std::cout << "SecHarmonic:\n"; printResult((secHarmonicResult));
    geo.join();
    std::cout << "Geo:\n"; printResult((geoResult));

}