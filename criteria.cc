#include "criteria.h"
#include "goal.h"
#include "tile.h"
#include "player.h"
using namespace std;


// constructor initializes all fields to given values
Criteria::Criteria(int id, const vector<Goal *> &myGoals,
                   const int coeff,
                   const ResourceArray &buildCost, const Player *owner)
    : id{id}, myGoals{myGoals}, coeff{coeff},
      buildCost{buildCost}, owner{owner}, resourceChange{buildCost} {}


Criteria::Criteria(const Criteria &other, const int coeff,
                   const ResourceArray buildCost)
    : Subject{other}, id{other.id}, myGoals{other.myGoals}, coeff{coeff}, 
      buildCost{buildCost}, owner{other.owner},
      resourceChange{array<int, 5>{}} {}
/* CONSTRUCTORS END HERE */


/* UPGRADES START HERE */
unique_ptr<Criteria> Criteria::upgradeCriteria(Player *player,
                                               bool beginPhase /* = false */,
                                               bool loadFromSave /* = false */) {
    if(beginPhase&&isBuilt()&&!loadFromSave){
        throw(string{"You cannot build on top of a completed criterion at this stage."});
    }
    if (!beginPhase) {
        isValidUpgrade(player); // throws specific exn if not
        updateResourceChange(buildCost);
    }
    if(!isOwnedBy(player)) { // if upgrading from noCriteria
        attach(player);
        addOwner(player);
        if(!loadFromSave&&!beginPhase) {
            try { // exception safety
                notifyObservers();
            }
            catch (string &str) {
                detach(player);
                owner = nullptr;
                throw;
            }
        }
    } else if(!loadFromSave&&!beginPhase) notifyObservers(); // update player resources
    unique_ptr<Criteria> newCriteria = newUpgradedCriteria();
    for(auto &goal : myGoals) {
        goal->detachCriteria(this);
        goal->attachCriteria(newCriteria.get());
    }
    newCriteria->notifyObservers(); // update textDisplay
    return newCriteria;
}

void Criteria::goalIsOwnedBy(Player *player) const {
    for(auto goal : myGoals) if(goal->isOwnedBy(player)) return;
    throw(string{"You are not connected to this criterion."});
}

void Criteria::nbrCriteriaExist() const {
    for(auto goal : myGoals) {
        if(goal->checkCriteriaExist()) {
            throw(string{"There is a neighboring criteria here. You cannot build."});
        }
    }
}
/* UPGRADES END HERE */


/* OBSERVER PATTERN STARTS HERE */
void Criteria::notifyObservers() {
    for(auto ob : observers) ob->notify(*this);
}


void Criteria::notify(Tile &tile) {
    updateResourceChange(tile.getResourceYield()*coeff);
    notifyObservers();
}


ResourceArray Criteria::getResourceChange() {
    return resourceChange;
}

int Criteria::level() const {
    return coeff;
}
/* OBSERVER PATTERN ENDS HERE */



/* GENERAL STARTS HERE */
int Criteria::getID() const {
    return id;
}

bool Criteria::isBuilt() const {
    return true;
}

bool Criteria::isOwnedBy(Player *player) const {
    return owner==player;
}

bool Criteria::nbrGoalOwnedBy(Player *player) const {
    for(auto g : myGoals) if(g->isOwnedBy(player)) return true;
    return false;
}

void Criteria::updateResourceChange(const ResourceArray &r) {
    resourceChange = r;
}

/* GENERAL ENDS HERE */

void Criteria::addOwner(Player *player) {
    owner = player;
}
