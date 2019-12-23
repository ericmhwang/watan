#include "resourceArray.h"
using namespace std;

ResourceArray::ResourceArray(int index)
    : theArray{array<int, 5>{}} {
        theArray[index] = 1;
    }

ResourceArray::ResourceArray(const std::array<int, 5> &theArray)
    : theArray{theArray} {}


int ResourceArray::operator[](int idx) const {
    return theArray[idx];
}

ResourceArray ResourceArray::operator+(const ResourceArray &other) const {
    array<int, 5> newArray;
    for(int i = 0; i < 5; ++i) newArray[i] = theArray[i] + other[i];
    return ResourceArray{newArray};
}

ResourceArray ResourceArray::operator*(const int coeff) const {
    array<int, 5> newArray;
    for(int i = 0; i < 5; ++i) newArray[i] = theArray[i] * coeff;
    return ResourceArray{newArray};
}


int ResourceArray::sum() const {
    int soFar = 0;
    for(int i = 0; i < 5; ++i) soFar += theArray[i];
    return soFar;
}

int ResourceArray::tileKind() const {
    int kind = CAFFEINE;
    for(; theArray[kind]==0; ++kind);
    return kind;
}

bool ResourceArray::isPositive() const {
    for(int i=0; i<5; ++i) if(theArray[i] < 0) return 0;
    return 1;
}

array<int, 5> ResourceArray::getArray() const {
    return theArray;
}
