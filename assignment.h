#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H
#include "criteria.h"
#include "midterm.h"

class NoCriteria;

class Assignment : public Criteria {
  public:
    // load from save constructor
    Assignment(int id, const std::vector<Goal *> &myGoals,
               Player *owner);
    // upgrade constructor
    Assignment(const NoCriteria &orig);
    void isValidUpgrade(Player *player) const override;
    std::unique_ptr<Criteria> newUpgradedCriteria() const override;
    std::string print() const override;
    ~Assignment() override = default;
};

#endif
