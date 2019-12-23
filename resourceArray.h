#ifndef RESOURCEARRAY_H
#define RESOURCEARRAY_H
#include <array>

const int CAFFEINE = 0;
const int LAB = 1;
const int LECTURE = 2;
const int STUDY = 3;
const int TUTORIAL = 4;


class ResourceArray {
    std::array<int, 5> theArray; // int[5]
  public:
    ResourceArray() {}
    explicit ResourceArray(int index); // sets index to 1, others to 0
    explicit ResourceArray(const std::array<int, 5> &theArray); // sets all values to values in theArray
    // accessor, so we dont need to do RA.theArray[1]
    int operator[](int idx) const; 
    ResourceArray operator+(const ResourceArray &other) const; // vector addition
    ResourceArray operator*(const int coeff) const; // scalar multiply by coeff
    int sum() const;
    int tileKind() const;
    bool isPositive() const; // if any indicies are negative
    std::array<int, 5> getArray() const;
};

#endif
