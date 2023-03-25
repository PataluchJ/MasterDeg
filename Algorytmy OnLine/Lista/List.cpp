#include "List.hpp"

std::ostream& operator<<(std::ostream& os, const CountableValue& cv)
{
    os << "{" << cv.v << "," << cv.count << "}";
    return os;
}