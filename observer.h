#ifndef OBSERVER_H
#define OBSERVER_H
#include <vector>

class Goal;
class Criteria;
class Tile;

class Observer {
  public:
    virtual void notify(Goal &goal);
    virtual void notify(Criteria &criteria);
    virtual void notify(Tile &tile);
    virtual ~Observer() = 0;
};

#endif
