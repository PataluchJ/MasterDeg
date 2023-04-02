#include "List.hpp"
#include "Distribution.hpp"

#include <string>
#include <thread>
#include <iostream>
#include <functional>

struct Result{
    double averageCost {0.0};
};

struct TestData{
    std::unique_ptr<ListAccesser> list;
    std::shared_ptr<DiscreteDistribution> dist;
    std::shared_ptr<Result> result;
    std::string name;
    size_t n;
    size_t repeats { 1000 };

    TestData(
        std::unique_ptr<ListAccesser> list,
        std::shared_ptr<DiscreteDistribution> dist,
        std::shared_ptr<Result> result,
        std::string name,
        size_t n
    ):
        list(std::move(list)),
        dist(dist),
        result(result),
        name(name),
        n(n) {}
};

using DataPointer = std::unique_ptr<TestData>;

template<typename ListType>
std::unique_ptr<ListAccesser> create(std::shared_ptr<DiscreteDistribution> dist){
    return std::move(std::make_unique<ListType>(ListType(dist)));
}

void test(DataPointer data){
    size_t cost = 0;

    for(size_t r = 0; r < data->repeats; r++){
        data->list->clear();
        for(size_t i = 0; i < data->n; i++)
            cost += data->list->access(data->dist->getRandom());
    }

    data->result->averageCost = (double)(cost) / (double)(data->n * data->repeats);
    std::cout << (data->name + " done.\n");
}


int main(){ 
    std::vector<DataPointer> data;
    std::vector<std::shared_ptr<Result>> results;
    std::vector<std::string> resultNames;
    std::vector<size_t> ns {100, 500, 1000, 5000, 10000, 50000, 100000};

    using ListAccesserCreator = std::function<std::unique_ptr<ListAccesser>()>;
    
    auto crossForTestSize = [&data, &results, &resultNames, &ns](std::string name, ListAccesserCreator listCreator, std::shared_ptr<DiscreteDistribution> dist){
        for(auto n: ns){
            std::string fullName = name + std::to_string(n);
            results.push_back(std::make_shared<Result>(Result()));
            resultNames.push_back(fullName);
            data.push_back(
                std::make_unique<TestData>(TestData(
                    std::move(listCreator()),
                    dist,
                    results.back(),
                    fullName,
                    n
                ))
            );

        }
    };
    auto crossForDistribution = [&crossForTestSize](std::string name, ListAccesserCreator listCreator){
        auto uniform = std::make_shared<DiscreteDistribution>(
            DiscreteDistribution(std::make_shared<UniformProbability>(UniformProbability()))
        );
        auto harmonic = std::make_shared<DiscreteDistribution>(
            DiscreteDistribution(std::make_shared<HarmonicProbability>(HarmonicProbability()))
        );
        auto secondHarmonic = std::make_shared<DiscreteDistribution>(
            DiscreteDistribution(std::make_shared<SecondHarmonicProbability>(SecondHarmonicProbability()))
        );
        auto geometric = std::make_shared<DiscreteDistribution>(
            DiscreteDistribution(std::make_shared<GeometricProbability>(GeometricProbability()))
        );

        crossForTestSize(name + "_uniform_", listCreator, uniform);
        crossForTestSize(name + "_harmonic_", listCreator, harmonic);
        crossForTestSize(name + "_secondHarmonic_", listCreator, secondHarmonic);
        crossForTestSize(name + "_geometric_", listCreator, geometric);
    };

    auto generateData = [&crossForDistribution](){
        ListAccesserCreator simple = [](){
            return std::make_unique<Simple>(Simple());
        };
        ListAccesserCreator moveToFront = [](){
            return std::make_unique<MoveToFront>(MoveToFront());
        };
        ListAccesserCreator count = [](){
            return std::make_unique<Count>(Count());
        };
        ListAccesserCreator transpose = [](){
            return std::make_unique<Transpose>(Transpose());
        };

        crossForDistribution("Simple", simple);
        crossForDistribution("MoveToFront", moveToFront);
        crossForDistribution("Count", count);
        crossForDistribution("Transpose", transpose);
    };

    std::cout << "Generating test configs...";
    generateData();

    std::cout << " created " << data.size() << " tests.\n";
    
    std::vector<std::thread> threads;
    for(size_t i = 0; i < data.size(); i++){
        threads.push_back(
            std::thread(
                test, std::move(data[i])
            )
        );
    }

    for(auto& th : threads){
        th.join();
    }
    //*/

    for(size_t i = 0; i < results.size(); i++){
        std::cout << resultNames[i] << ": " << results[i]->averageCost << "\n";
    }
    return 0;
}