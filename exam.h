#ifndef EXAM_H
#define EXAM_H
#include "criteria.h"

class Midterm;

class Exam : public Criteria {
  public:
    // load from save constructor
    Exam(int id, const std::vector<Goal *> &myGoals, Player *owner);
    // upgrade from midterm constructor
    Exam(const Midterm &orig);
    void isValidUpgrade(Player *player) const override;
    std::unique_ptr<Criteria> newUpgradedCriteria() const override;
    std::string print() const override;
    ~Exam() override = default;
};

#endif
