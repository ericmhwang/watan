#ifndef MIDTERM_H
#define MIDTERM_H
#include "criteria.h"

class Assignment;

class Midterm : public Criteria {
  public:
    // load from save constructor
    Midterm(int id, const std::vector<Goal *> &myGoals,
            Player *owner);
    // upgrade from assignment constructor
    Midterm(const Assignment &orig);
    void isValidUpgrade(Player *player) const override;
    std::unique_ptr<Criteria> newUpgradedCriteria() const override;
    std::string print() const override;
    ~Midterm() override = default;
};

#endif
