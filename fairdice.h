#ifndef FAIRDICE_H
#define FAIRDICE_H
#include <random>
#include "dice.h"

class FairDice : public Dice {
    std::default_random_engine *eng;
  public:
    FairDice(std::default_random_engine *eng);
    int roll() override;
};

#endif
