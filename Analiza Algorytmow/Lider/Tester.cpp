#include "LeaderElection.hpp"

#include <memory>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>

void saveResultForHisto(std::shared_ptr<Algorithm> alg, NodeSet& set, std::string filepath, size_t runs = 500000){
    std::cout << "Started run for " << filepath << " ";
    std::fstream file(filepath, std::ios_base::out);
    if(!file.is_open()){
        std::cerr << "Unable to open file " << filepath << " for saving output!\n";
        return;
    }

    std::map<size_t, size_t> results;
    size_t maxSlot = 0;
    size_t noticeInterval = runs / 10;
    for(size_t r = 0; r < runs; r++){
        auto result = alg->simulate(set);
        if(result > maxSlot) 
            maxSlot = result;
        if(results.find(result) != results.end())
            results[result]++;
        else 
            results[result] = 1;

        if(r % noticeInterval == 0){
            std::cout << "#";
            std::cout.flush();
        }
            
    }

    
    for(size_t i = 0 ; i <= maxSlot; i++){
        size_t count = 0;
        if(results.find(i) != results.end())
            count = results[i];
        file <<  i << ";" << count << "\n";
    }

    std::cout << " finished.\n";
    file.close();
}

void generateDataForHistogram(){
    const size_t nodeAmount = 50000;
    const size_t nodeBound = 50000;

    auto setAmount = std::make_shared<SetAmount>(SetAmount(nodeAmount));
    auto upperBound = std::make_shared<UpperBound>(UpperBound(nodeBound));
    
    /* Known amount */
    NodeSet set1(nodeAmount);
    saveResultForHisto(setAmount, set1, "knowAmountTest.csv");

    /* Known bound, n = 2   */
    NodeSet set2(2);
    saveResultForHisto(upperBound, set2, "boundN2.csv");

    /* Known bound, n = u/2 */
    NodeSet set3(nodeBound / 2);
    saveResultForHisto(upperBound, set3, "boundUdiv2.csv");

    /* Known bound, n = u   */
    NodeSet set4(nodeBound);
    saveResultForHisto(upperBound, set4, "boundNeqU.csv");
}

void generateDataForSLN(){
    std::fstream file("slnData.csv", std::ios_base::out);
    if(!file.is_open()){
        std::cerr << "Unable to open file slnData.csv for saving output!\n";
        return;
    }
    std::cout << "Starter generateDataForSLN... ";
    /* Worst case scenarion when u = n */
    const size_t nodeAmount = 50000;
    const size_t roundLenght = round(log2(nodeAmount) + 0.5);
    std::cout << "Round len: " << roundLenght << "\n";
    const size_t runs = 50000;
    NodeSet set(nodeAmount);
    auto upperBound = std::make_shared<UpperBound>(UpperBound(nodeAmount));

    size_t max = 0;
    size_t noticeInterval = runs / 10;
    size_t succ = 0;

    std::map<size_t, size_t> results;
    for(size_t r = 0; r < runs; r++){
        if(r % noticeInterval == 0){
            std::cout << "#";
            std::cout.flush();
        }

        if(upperBound->oneRound(set))
            succ++;
    }

    file << succ << " out of " << runs << " p = " << (double)(succ)/(double)(runs);

    std::cout << " finished.\n";
    std::cout << succ << " out of " << runs << " p = " << (double)(succ)/(double)(runs);
    file.close();
}   

int main(int argc, char** argv){
    generateDataForSLN();
}