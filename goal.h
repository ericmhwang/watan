#ifndef GOAL_H
#define GOAL_H
#include <vector>
#include <string>
#include "subject.h"
#include "resourceArray.h"

class Criteria;
class Player;

class Goal : public Subject {
    int id;
    std::vector<Criteria *> myCriteria;
    Player *owner;
    const ResourceArray buildCost{std::array<int, 5>{{0, 0, 0, -1, -1}}};
  public:
    Goal(int id, Player *owner = nullptr);
    // initializing the board
    void attachCriteria(Criteria * crit);
    void detachCriteria(Criteria * oldCrit);
    int getID() const;
    void buildGoal(Player *player);
    void notifyObservers() override;
    ResourceArray getResourceChange() const;
    std::string print() const;
    // for use by criteria
    bool checkCriteriaExist() const;
    bool isOwnedBy(Player *player) const;
    ~Goal() override = default;
};

#endif
