#include "Types.hpp"

CountableVariable::CountableVariable(Variable v) : v(v), count(1) {}

std::ostream& operator<<(std::ostream& os, const CountableVariable& cv)
{
    os << "{" << cv.v << "," << cv.count << "}";
    return os;
}
