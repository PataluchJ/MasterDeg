#include <memory>
#include <list>
#include <functional>
#include <algorithm>

#include "Types.hpp"

class ListAccesser{
    public:
    virtual ~ListAccesser() {}
    virtual Cost access(Variable value) = 0;
    virtual void clear() = 0;
};

template<typename InternalListType>
class TypableListAccesser : public ListAccesser {
    public:

    using EqualFunction = std::function<bool(const InternalListType&, const Variable&)>;
    using ListType = typename std::list<InternalListType>;
    using ListIterator = typename ListType::iterator;

    TypableListAccesser(EqualFunction equalFunction)
        : eq(equalFunction) {}
    virtual ~TypableListAccesser() = default;

    Cost access(Variable value) override {
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
    void clear() { list.clear(); }
    std::list<InternalListType> list;
    protected:
    const EqualFunction eq;
    virtual void onAccess(ListIterator& it) = 0;
};

class Simple : public TypableListAccesser<Variable> {
    public:
    Simple();
    ~Simple();

    static std::shared_ptr<ListAccesser> create();
    private:
    void onAccess(ListIterator& it) override;

};

class MoveToFront : public TypableListAccesser<Variable> {
    public:
    MoveToFront();
    ~MoveToFront();
    
    static std::shared_ptr<ListAccesser> create();
    private:
    void onAccess(ListIterator& it);
};

class Transpose : public TypableListAccesser<Variable> {
    public:
    Transpose();
    ~Transpose();

    static std::shared_ptr<ListAccesser> create();

    private:
    void onAccess(ListIterator& it) override;
};

class Count : public TypableListAccesser<CountableVariable> {
    public:
    Count();
    ~Count();
    static std::shared_ptr<ListAccesser> create();
    private:
    void onAccess(ListIterator& it) override;
};