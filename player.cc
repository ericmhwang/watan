#include <sstream>
#include <iostream>
#include <random>
#include "player.h"
#include "criteria.h"
#include "goal.h"
#include "dice.h"
#include "loadeddice.h"
#include "fairdice.h"
using namespace std;

default_random_engine generator; // random number generator
string resourcelst[5] = {"caffeines", "labs", "lectures", "studies", "tutorials"};

// new game constructor
Player::Player(string _name, default_random_engine *_eng): 
    name {_name}, 
    resources {array<int, 5> {{0, 0, 0, 0, 0}}},
    eng{_eng},
    dice {make_unique<FairDice>(eng)} {}

// load game constructor
Player::Player(string _name, ResourceArray _resources, 
               default_random_engine *_eng) : 
    name {_name},
    resources {_resources},
    eng{_eng},
    dice {make_unique<FairDice>(eng)} {} 

// get resource method to be used by the board when outputting resource gained
ResourceArray Player::getResources() const {
    return resources;
}

// status output
string Player::status() const {
    string retStr;
    ostringstream oss;
    oss << calculateScore();
    retStr = name + " has " + oss.str() + " course criteria, ";
    for (int i = 0; i < 5; ++i) {
        ostringstream val;
        val << resources[i];
        retStr += val.str();
        if (i == 0) retStr += " caffeines, ";
        else if (i == 1) retStr += " labs, ";
        else if (i == 2) retStr += " lectures, ";
        else if (i == 3) retStr += " studies, and ";
        else retStr += " tutorials.";
    }
    return retStr;
}

// criteria output
string Player::getCriteria() const {
	string output{name + " has completed:"};
	for (pair<int, int> crit : criteria) {
		output += "\n" + to_string(crit.first) + " " + to_string(crit.second);	
	}
	return output;
}

string Player::save() const {
    string retStr;
    for (int i = 0; i < 5; ++i) {
        ostringstream val;
        val << resources[i];
        retStr += val.str() + " ";
    }
    retStr += "g ";
    for (auto &it : goals) {
        ostringstream goal;
        goal << it;
        retStr += " " + goal.str() + " ";
    }
    retStr += "c ";
    for (auto &it : criteria) {
        ostringstream crit;
        ostringstream level;
        crit << it.first;
        level << it.second;
        retStr += " " + crit.str() + " " + level.str();
    }
    return retStr;
}

// calculates total criterion FIX!!!!!!!
int Player::calculateScore() const {
    int score = 0;
    for (auto &it : criteria) {
        score += it.second;
    }
    return score;
}


bool Player::haveEnoughResources(ResourceArray myResources) const{
    ResourceArray tmp = myResources + resources;
    return tmp.isPositive();
}

void Player::updateResources(ResourceArray myResources) {
    resources = resources + myResources;
}


void Player::notify(Criteria & myCriteria) {
    if(haveEnoughResources(myCriteria.getResourceChange())) {
        updateResources(myCriteria.getResourceChange());
        int id = myCriteria.getID();
        bool found = false;
        if (myCriteria.getResourceChange().sum() <= 0) {
            for (auto &it : criteria) {
                if (it.first == id) {
                    it.second = myCriteria.level();
                    found = true;
                    break;
                }
            }
            if (!found) criteria.emplace_back(pair<int,int> {id, 1});
        }
    } else throw(string{"You do not have enough resources."});
}

// do the same for the goal
void Player::notify(Goal & myGoal) {
    ResourceArray resourceChange {{0, 0, 0, -1, -1}};
    if (haveEnoughResources(resourceChange)) {
        updateResources(resourceChange);
        int id = myGoal.getID();
        goals.emplace_back(id);
    } else throw(string{"You do not have enough resources."});
}

// generates a "GooseArray" which indicates the resources lost to the goose
ResourceArray Player::gooseArray() {
    int count = resources.sum() / 2;
    array<int, 5> arr{resources.getArray()};
    vector<int> randomVec;
    array<int, 5> loseArr {};
    for (int i = 0; i < 5; ++i) {
        for (int j = arr[i]; j > 0; --j) randomVec.emplace_back(i);
    }
    while (count > 0) {
        uniform_int_distribution<int> dist(0, randomVec.size() - 1);
        int rand = dist(*eng);
        --loseArr[randomVec[rand]];
        randomVec.erase(randomVec.begin() + rand);
        --count;
    }
    return ResourceArray {loseArr};
}

string Player::goose() {
    string retStr;
    ostringstream oss;
    int resourcesTotal = resources.sum();
    if (resourcesTotal >= 10) {
        ResourceArray gooseArr = gooseArray();
        string lost = to_string(resourcesTotal / 2);
        retStr = "Student " + name + " loses " + lost + 
            " resources to the geese. They lose: " + "\n";
        for (int i = 0; i < 5; ++i) {
            if (gooseArr[i] < 0) {
                string num = to_string(-gooseArr[i]);
                retStr += num + " " + resourcelst[i] + "\n";
            }
        }
        updateResources(gooseArr);
    }
    return retStr;
}

void Player::loaded(int val) {
    dice = make_unique<LoadedDice>(val);
}

void Player::fair() {
    dice = make_unique<FairDice>(eng);
}

int Player::roll() {
    return dice->roll();
}

void Player::trade(ResourceArray tradeArr) {
    updateResources(tradeArr);
}

string Player::print() const {
    return name;
}

string Player::steal(Player * from) {
    vector<int> resourceV;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < from->resources[i]; ++j) {
            resourceV.emplace_back(i);
        }
    }
    int cap = resourceV.size() - 1;
    uniform_int_distribution<int> dist(0, cap);
    int rn = resourceV[dist(*eng)];
    ResourceArray rArr {rn};
    updateResources(rArr);
    from->updateResources(rArr * -1);
    if (rn == 0) return "caffeine ";
    else if (rn == 1) return "lab ";
    else if (rn == 2) return "lecture ";
    else if (rn == 3) return "study ";
    else return "tutorial ";
}

void Player::addGoal(int id) {
    goals.emplace_back(id);
}
