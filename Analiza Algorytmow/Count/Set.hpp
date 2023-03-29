#pragma once

using SetElement = unsigned int;

class SetElementGenerator{
    public:
    virtual ~SetElementGenerator() = default;

    virtual SetElement next() = 0;
    virtual unsigned int remaining() = 0;
    virtual unsigned int size() = 0;
    virtual void reset() = 0;
};

class RangeBasedSetGenerator : public SetElementGenerator {
    public:
    RangeBasedSetGenerator(SetElement first, SetElement last);

    SetElement next() override;
    unsigned int remaining() override;
    unsigned int size() override;
    void reset() override;

    private:
    SetElement first;
    SetElement current;
    SetElement last;
};