#include "List.hpp"
#include <iostream>

template<typename T>
void printList(std::list<T>& list){
    for(auto it = list.begin(); it != list.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << "\n";
}

int main(){
    Count f;
    f.access(1);
    printList<CountableValue>(f.list);
    f.access(1);
    printList<CountableValue>(f.list);
    f.access(1);
    printList<CountableValue>(f.list);
    f.access(2);
    printList<CountableValue>(f.list);
    f.access(3);
    printList<CountableValue>(f.list);
    f.access(3);
    printList<CountableValue>(f.list);
    f.access(2);
    printList<CountableValue>(f.list);
    f.access(2);
    printList<CountableValue>(f.list);
    f.access(2);
    printList<CountableValue>(f.list);
    return 0;
}