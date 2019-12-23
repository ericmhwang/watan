#include "loadeddice.h"
using namespace std;

LoadedDice::LoadedDice(int val)
    : value{val} {}

int LoadedDice::roll() {
    return value;
}
