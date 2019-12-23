#include "fairdice.h"
using namespace std;

FairDice::FairDice(default_random_engine *eng)
    : eng{eng} {}

int FairDice::roll() {
    uniform_int_distribution<> dist(1, 6);
    return dist(*eng) + dist(*eng);
}
