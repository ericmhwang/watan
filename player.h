#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <string>
#include <memory>
#include <random>
#include "dice.h"
#include "observer.h"
#include "resourceArray.h"

class Dice;
class Criteria;
class Goal;

class Player : public Observer {
    std::string name;
    ResourceArray resources;
    std::vector<std::pair<int, int>> criteria; // index to level
    std::vector<int> goals;
    std::default_random_engine *eng;
    std::unique_ptr<Dice> dice;
    void updateResources(ResourceArray myResources);
    ResourceArray gooseArray();
  public:
    Player(std::string _name, std::default_random_engine *eng);
    Player(std::string _name,
            ResourceArray _resources, 
            std::default_random_engine *eng); // constructor for loading game
    ResourceArray getResources() const;
    std::string status() const;
    std::string getCriteria() const;
    std::string save() const;
    int calculateScore() const;
    bool haveEnoughResources(ResourceArray myResources) const;
    void notify(Criteria & myCriteria) override;
    void notify(Goal & myGoal) override;
    std::string goose();
    void loaded(int val);
    void fair();
    int roll();
    void trade(ResourceArray tradeArr);
    std::string print() const;
    // steal method for geese, takes in a player that currPlayer steals from
    std::string steal(Player * from);
    // attaches the goal into the vector when loading
    void addGoal (int id);
};

#endif
