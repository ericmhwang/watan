#ifndef NOCRITERIA_H
#define NOCRITERIA_H
#include "criteria.h"

class NoCriteria : public Criteria {
  public:
    NoCriteria(int id, const std::vector<Goal *> &myGoals);
    void isValidUpgrade(Player *player) const override;
    std::unique_ptr<Criteria> newUpgradedCriteria() const override;
    bool isBuilt() const override;
    std::string print() const override;
    ~NoCriteria() override = default;
};


#endif
