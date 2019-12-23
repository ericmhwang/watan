#include <sstream>
#include "tile.h"
#include "observer.h"


std::string Tile::savePrint() const {
    std::ostringstream ind;
    std::ostringstream val;
    ind << resourceYield.tileKind();
    val << value;
    return ind.str() + " " + val.str();
}


Tile::Tile(std::vector<Observer *> obList, int id, int resource, int value)
    : id{id}, resourceYield{resource}, value{value} {
        for(auto ob : obList) attach(ob);
}


int Tile::getID() const {
    return id;
}


bool Tile::valueEqual(int val) const {
    if(value==val) return true;
    else return false;
}


ResourceArray Tile::getResourceYield() const {
    return resourceYield;
}


void Tile::notifyObservers() {
    for(auto ob : observers) ob->notify(*this);
}
