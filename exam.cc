#include "midterm.h"
#include "exam.h"
#include "player.h"
using namespace std;

const ResourceArray buildCostInvalid{array<int, 5>{{0, 0, 0, 0, 0}}};

Exam::Exam(const Midterm &orig)
    : Criteria{dynamic_cast<const Criteria &>(orig), 3, buildCostInvalid} {}


void Exam::isValidUpgrade(Player *player) const {
    // check if this is owned by another player
    throw(string{"You cannot upgrade an exam!"});
}


unique_ptr<Criteria> Exam::newUpgradedCriteria() const {
    throw(string{"This should never throw! There is a problem with upgradeCriteria"});
}

string Exam::print() const {
    return owner->print().substr(0, 1) + "E";
}
