#include <memory>
#include <list>
#include <functional>
#include <algorithm>
#include <ostream>

using Value = uint32_t;
using Cost = uint32_t;

struct CountableValue{
    CountableValue(Value v) : v(v), count(1) {}
    Value v;
    size_t count;

    friend std::ostream& operator<<(std::ostream& os, const CountableValue& dt);
};

class ListAccesser{
    public:
    virtual ~ListAccesser() {}
    virtual Cost access(Value value) = 0;
};

template<typename InternalListType>
class TypableListAccesser : public ListAccesser {
    public:

    using EqualFunction = std::function<bool(const InternalListType&, const Value&)>;
    using ListType = typename std::list<InternalListType>;
    using ListIterator = typename ListType::iterator;

    TypableListAccesser(EqualFunction equalFunction)
        : eq(equalFunction) {}
    virtual ~TypableListAccesser() = default;

    Cost access(Value value) override {
        auto current = list.begin();
        auto end = list.end();
        Cost accessed = 0;

        while(current != end){
            accessed++;
            if(eq(*current, value)){
                onAccess(current);
                return accessed;
            }
            current++;
        }

        list.push_back(InternalListType(value));
        // onAccess((--list.end()));
        return accessed;
    }

    std::list<InternalListType> list;
    protected:
    const EqualFunction eq;
    virtual void onAccess(ListIterator& it) = 0;
};

class Simple : public TypableListAccesser<Value> {
    public:
    Simple() : TypableListAccesser<Value>(EqualFunction([](const Value& a, const Value& b){return a == b;})) {}
    ~Simple() {}

    static std::shared_ptr<ListAccesser> create() {
        return std::make_shared<Simple>(Simple());
    }
    private:
    void onAccess(ListIterator& it) override {}

};

class MoveToFront : public TypableListAccesser<Value> {
    public:
    MoveToFront() : TypableListAccesser<Value>(EqualFunction([](const Value& a, const Value& b){return a == b;})) {}
    ~MoveToFront() = default;
    
    static std::shared_ptr<ListAccesser> create() {
        return std::make_shared<MoveToFront>(MoveToFront());
    }
    private:
    void onAccess(ListIterator& it) override {
        auto temp = (*it);
        auto nextTemp = (*(list.begin()));
        auto current = list.begin();
        while(current != it){
            (*current) = temp;
            temp = nextTemp;
            ++current;
            nextTemp = (*current);
        }
        (*it) = temp;
    }
};

class Transpose : public TypableListAccesser<Value> {
    public:
    Transpose() : TypableListAccesser<Value>(EqualFunction([](const Value& a, const Value& b){return a == b;})) {}
    ~Transpose() = default;

    static std::shared_ptr<ListAccesser> create() {
        return std::make_shared<Transpose>(Transpose());
    }

    private:
    void onAccess(ListIterator& it) override {
        if(it == list.begin())
            return;
        auto prev = it;
        --prev;
        auto temp = (*prev);
        (*prev) = (*it);
        (*it) = temp;
    }
};

class Count : public TypableListAccesser<CountableValue> {
    public:
    Count() : TypableListAccesser<CountableValue>(EqualFunction([](const CountableValue& a, const Value& b){return a.v == b;})) {}
    ~Count() = default;
    static std::shared_ptr<ListAccesser> create() {
        return std::make_shared<Count>(Count());
    }
    private:
    void onAccess(ListIterator& it) override {
        (*it).count++;
        list.sort([](const CountableValue& a, const CountableValue& b) { return a.count > b.count; });
    }
};