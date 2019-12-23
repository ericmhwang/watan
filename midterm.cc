#include "assignment.h"
#include "midterm.h"
#include "exam.h"
#include "player.h"
using namespace std;

const ResourceArray buildCostExam{array<int, 5>{{-3, -2, -2, -1, -2}}};

Midterm::Midterm(const Assignment &orig)
    : Criteria{dynamic_cast<const Criteria &>(orig), 2, buildCostExam} {}


void Midterm::isValidUpgrade(Player *player) const {
    if(!isOwnedBy(player)) throw(string{"You cannot update a criterion you do not own."});
}


unique_ptr<Criteria> Midterm::newUpgradedCriteria() const {
    unique_ptr<Criteria> exam{new Exam{*this}};
    return exam;
}

string Midterm::print() const {
    return owner->print().substr(0, 1) + "M";
}
