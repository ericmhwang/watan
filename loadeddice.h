#ifndef LOADEDDICE_H
#define LOADEDDICE_H
#include <random>
#include "dice.h"

class LoadedDice : public Dice {
    int value = 0;
  public:
    LoadedDice(int val);
    int roll() override;
};

#endif
