#ifndef TILE_H
#define TILE_H
#include <vector>
#include <string>
#include "subject.h"
#include "resourceArray.h"

class Criteria;

class Tile : public Subject {
    int id;
    ResourceArray resourceYield;
    int value;
  public:
    Tile(std::vector<Observer *>, int id, int resource, int value);
    int getID() const;
    std::string savePrint() const;
    bool valueEqual(int val) const;
    ResourceArray getResourceYield() const;
    void notifyObservers() override;
    ~Tile() override = default;
};

#endif
