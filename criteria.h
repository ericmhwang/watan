#ifndef CRITERIA_H
#define CRITERIA_H
#include <vector>
#include <memory>
#include <string>
#include "subject.h"
#include "observer.h"
#include "resourceArray.h"

class Goal;
class Tile;
class Player;

class Criteria : public Subject, public Observer {
    int id;
    std::vector<Goal *> myGoals;
    const int coeff;
    const ResourceArray buildCost;
  protected:
    const Player *owner;
    ResourceArray resourceChange;
  public:

    // constructor for setting up board
    Criteria(const int id, const std::vector<Goal *> &myGoals,
             const int coeff, const ResourceArray &buildCost, 
             const Player *owner);
    

    // constructor for upgrading criteria
    Criteria(const Criteria &other, const int coeff,
             const ResourceArray buildCost);

    // upgrade
    std::unique_ptr<Criteria> upgradeCriteria(Player *player,
                                              bool beginPhase = false,
                                              bool loadFromSave = false);
    // upgrade helpers
    // what makes a valid upgrade depend on subclass
  protected:
    virtual void isValidUpgrade(Player *player) const = 0;
    // what the criteria upgrades to depends on subclass
    virtual std::unique_ptr<Criteria> newUpgradedCriteria() const = 0;
    void goalIsOwnedBy(Player *player) const;
    void nbrCriteriaExist() const;
    void addOwner(Player *player);
  public:
    // observer pattern
    void notifyObservers() override;
    void notify(Tile &tile) override;
    ResourceArray getResourceChange();
    int level() const;
    
    // general
    int getID() const;
    void updateResourceChange(const ResourceArray &r);
    virtual std::string print() const = 0;

    // for use by goal
    bool isOwnedBy(Player *player) const;
    bool nbrGoalOwnedBy(Player *player) const;
    virtual bool isBuilt() const;

    // destructor
    virtual ~Criteria() override = default;
};

#endif
