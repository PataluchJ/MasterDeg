#pragma once

#include <functional>
#include <vector>
#include <thread>

/*
struct TestData {
    TestData() = default;
    virtual ~TestData() = default;
};
using TestDataPointer = std::unique_ptr<TestData>;
using TestDataVector = std::unique_ptr<std::vector<DataType>>;

template<typename DataType>
class ITestDataGenerator {
    public:
    ITestDataGenerator() = default;
    virtual ~ITestDataGenerator() = default;
    virtual std::unique_ptr<std::vector<DataType>> generateTestData() = 0;
};

template<typename DataType>
class ITest {
    public:
    ITest(std::unique_ptr<TestData> data)
        : data(std::move(data)) {}
    virtual ~ITest() = default;

    virtual void operator()() = 0;

    protected:
    std::unique_ptr<DataType> data;
};

class ITestBuilder {
    public:
    virtual ~ITestBuilder() = default;

    virtual std::unique_ptr<ITest> build(std::unique_ptr<TestData> data) = 0;
};

class TestRunner {
    public:
    TestRunner(std::shared_ptr<ITestDataGenerator> gen, std::shared_ptr<ITestBuilder> builder)
        : gen(gen), builder(builder) {}

    void run(){
        std::vector<std::unique_ptr<ITest>> tests;
        std::vector<std::thread> threads;

        auto allData = gen->generateTestData();
        for(auto& data : allData){
            tests.push_back(builder->build(std::move(data)));
            threads.push_back(std::thread((*tests.back())));
        }

        for(auto& thread : threads){
            thread.join();
        }
    }

    private:
    std::shared_ptr<ITestDataGenerator> gen;
    std::shared_ptr<ITestBuilder> builder;
    
    
};
*/