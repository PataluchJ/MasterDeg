#include "List.hpp"

Simple::Simple() 
    : TypableListAccesser<Variable>(EqualFunction([](const Variable& a, const Variable& b){return a == b;})) {}

Simple::~Simple() {}

std::shared_ptr<ListAccesser> Simple::create() {
    return std::make_shared<Simple>(Simple());
}

void Simple::onAccess(ListIterator& it) {}

MoveToFront::MoveToFront() 
    : TypableListAccesser<Variable>(EqualFunction([](const Variable& a, const Variable& b){return a == b;})) {}

MoveToFront::~MoveToFront() { }

std::shared_ptr<ListAccesser> MoveToFront::create() {
    return std::make_shared<MoveToFront>(MoveToFront());
}

void MoveToFront::onAccess(ListIterator& it) {
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

Transpose::Transpose() 
    : TypableListAccesser<Variable>(EqualFunction([](const Variable& a, const Variable& b){return a == b;})) {}

Transpose::~Transpose() {}

std::shared_ptr<ListAccesser> Transpose::create() {
    return std::make_shared<Transpose>(Transpose());
}

void Transpose::onAccess(ListIterator& it) {
    if(it == list.begin())
        return;
    auto prev = it;
    --prev;
    auto temp = (*prev);
    (*prev) = (*it);
    (*it) = temp;
}

Count::Count() 
    : TypableListAccesser<CountableVariable>(EqualFunction([](const CountableVariable& a, const Variable& b){return a.v == b;})) {}
    
Count::~Count() {}

std::shared_ptr<ListAccesser> Count::create() {
    return std::make_shared<Count>(Count());
}

void Count::onAccess(ListIterator& it) {
    (*it).count++;
    list.sort([](const CountableVariable& a, const CountableVariable& b) { return a.count > b.count; });
}
