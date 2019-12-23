#include "noCriteria.h"
#include "assignment.h"
using namespace std;

const ResourceArray buildCostAssn{array<int, 5>{{-1, -1, -1, -1, 0}}};

NoCriteria::NoCriteria(int id, const vector<Goal *> &myGoals)
    : Criteria{id, myGoals, 0, buildCostAssn, nullptr} {}


void NoCriteria::isValidUpgrade(Player *player) const {
    goalIsOwnedBy(player);
    nbrCriteriaExist();
}


unique_ptr<Criteria> NoCriteria::newUpgradedCriteria() const {
    unique_ptr<Criteria> assn{new Assignment{*this}};
    return assn;
}


bool NoCriteria::isBuilt() const {
    return false;
}

string NoCriteria::print() const {
    string s = to_string(getID());
    if(s.length()==1) s = " " + s;
    return s;
}
