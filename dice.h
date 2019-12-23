#ifndef DICE_H
#define DICE_H
#include <memory>

class Dice {
  public:
    virtual int roll() = 0;
    virtual ~Dice() = default;
};

#endif
