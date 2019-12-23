#include "noCriteria.h"
#include "assignment.h"
#include "midterm.h"
#include "player.h"
using namespace std;

const ResourceArray buildCostMid{array<int, 5>{{0, 0, -2, -3, 0}}};

Assignment::Assignment(const NoCriteria &orig)
    : Criteria{dynamic_cast<const Criteria &>(orig), 1, buildCostMid} {}


void Assignment::isValidUpgrade(Player *player) const {
    if(!isOwnedBy(player)) throw(string{"You cannot update a criterion you do not own."});
}


unique_ptr<Criteria> Assignment::newUpgradedCriteria() const {
    unique_ptr<Criteria> mid{new Midterm{*this}};
    return mid;
}

string Assignment::print() const {
    return owner->print().substr(0, 1) + "A";
}
