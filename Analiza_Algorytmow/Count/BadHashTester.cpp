#include <memory>

#include "Set.hpp"
#include "ConcreateHashFunction.hpp"
#include "MinCount.hpp"

#include <iostream>
#include <thread>
#include <string>
#include <fstream>

struct Result{
    Result(std::string name) 
        : name(name) {}
    size_t estimatedSize {0};
    std::string name{""};
};

struct TestData{
    TestData(
        std::shared_ptr<Result> result,
        std::shared_ptr<HashFunction> hash,
        std::shared_ptr<SetElementGenerator> gen,
        size_t k
    ) :
    result(result),
    hash(hash),
    gen(gen),
    k(k) {}

    std::shared_ptr<Result> result;
    std::shared_ptr<HashFunction> hash;
    std::shared_ptr<SetElementGenerator> gen;
    size_t k;
};

using DataPointer = std::unique_ptr<TestData>;
void printus(std::vector<double> x){
    for(auto v : x)
        std::cout << v << " ";
    std::cout << "\n";
}
void test(DataPointer data){
    MinCount mc(data->hash, data->k);
    SetElementGenerator& gen = (*data->gen);

    while(gen.remaining() > 0)
        mc.in(gen.next());

    data->result->estimatedSize = mc.getEstimation();
    //std::string endLog = data->result->name + "\t" + std::to_string(data->result->estimatedSize) + "\n";
    //std::cout << endLog;
}

int main(){
    std::vector<DataPointer> data;
    std::vector<std::shared_ptr<Result>> results;

    auto getTestData = [&data,&results](){
        std::vector<size_t> ks{100};
        
        size_t rangeStart = 1;
        size_t rangeStop = 1;
        for(size_t n = 1; n <= 10000 ; n++){
            rangeStart = rangeStop;
            rangeStop += n;
            for(auto k : ks){
                results.push_back(std::shared_ptr<Result>(new Result(std::to_string(n) + ";" + std::to_string(k))));
                data.push_back(std::unique_ptr<TestData>(new TestData(
                    results.back(),
                    std::shared_ptr<HashFunction>(new ShorterHash(24)),
                    std::shared_ptr<SetElementGenerator>(
                        new RangeBasedSetGenerator(rangeStart, rangeStop)
                        ),
                    k
                )));
            }           
        } 
    };

    std::cout << "Generating tests configs...";
    getTestData();
    std::cout << data.size() << " generated.\n";

    /* This kills the crab
    std::vector<std::thread> threads;

    std::cout << "Jobs starting...\n";
    for(auto& d : data){
        threads.push_back(std::thread(test, std::move(d)));
    }

    for(auto& t : threads){
        t.join();
    }
    //*/

    std::fstream out;
    out.open("bh_Shorter24.csv", std::fstream::ios_base::out);
    if(!out.is_open()){
        std::cout <<"Failed to open file for saving results!\n";
        return -1;
    }

    auto multi = [&data](size_t offset, size_t step){
        size_t poolSize = data.size() / step;
        size_t tested = 0;
        for(int i = offset; i < data.size(); i += step, tested++){
            test(std::move(data[i]));
            if(tested % (poolSize / 10) == 0){
                std::cout << "Thread " << offset << " at " << tested << " / " << poolSize << "\n";
            }
        }
    };

    std::vector<std::thread> threads;
    size_t off = 5; 
    for(size_t i = 0; i < off; i++){
        threads.push_back(std::thread(multi,i,off));
    }
    for(auto& t : threads){
        t.join();
    }
    std::cout << "Saving to file...\n";

    out << "n;k;m\n";
    for(auto& r : results){
        //std::cout << r->name << ";" << r->estimatedSize << "\n";
        out << r->name << ";" << r->estimatedSize << "\n";
    }

    out.close();
    return 0;
}