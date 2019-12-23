#include "goal.h"
#include "criteria.h"
#include "player.h"
using namespace std;


Goal::Goal(int id, Player *owner /* = nullptr */) :
    id {id}, owner {owner} {}

void Goal::attachCriteria(Criteria *crit) {
    myCriteria.push_back(crit);
}

void Goal::detachCriteria(Criteria *crit) {
    auto it = myCriteria.begin();
    for(; *it != crit; ++it);
    myCriteria.erase(it);
}

int Goal::getID() const {
    return id;
}

bool Goal::isOwnedBy(Player *player) const {
    return owner==player;
}

bool Goal::checkCriteriaExist() const {
    for(auto c : myCriteria) if(c->isBuilt()) return true;
    return false;
}

void Goal::buildGoal(Player *player) {
    if(owner) throw(string{"You cannot achieve an already-acheived goal"});
    bool connected = false;
    for(auto c : myCriteria) {
        if(c->isOwnedBy(player)||c->nbrGoalOwnedBy(player)) {
            connected = true;
            break;
        }
    }
    if(!connected) throw(string{"You cannot reach this goal."});
    attach(player);
    try {
        owner = player;
        notifyObservers();
    }
    catch (string &str) {
        detach(player);
        owner = nullptr;
        notifyObservers();
        throw;
    }
}

string Goal::print() const {
    string s;
    if(owner) {
        return owner->print().substr(0, 1) + "A";
    } else {
        s = to_string(getID());
        if(s.length()==1) s = " " + s;
    }
    return s;
}

void Goal::notifyObservers() {
    for(auto ob : observers) ob->notify(*this);
}
