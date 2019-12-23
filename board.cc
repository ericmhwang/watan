#include <fstream>
#include <sstream>
#include "board.h"
#include "textdisplay.h"
#include "noCriteria.h"
using namespace std;

// make big const with criteria in each tile to attach it to tiles
const array<array<int, 6>, 19> tilesCriteria { {
    {0, 1, 3, 4, 8, 9}, {2, 3, 7, 8, 13, 14}, {4, 5, 9, 10, 15, 16},
    {6, 7, 12, 13, 18, 19}, {8, 9, 14, 15, 20, 21}, {10, 11, 16, 17, 22, 23},
    {13, 14, 19, 20, 25, 26}, {15, 16, 21, 22, 27, 28}, {18, 19, 24, 25, 30, 31},
    {20, 21, 26, 27, 32, 33}, {22, 23, 28, 29, 34, 35}, {25, 26, 31, 32, 37, 38},
    {27, 28, 33, 34, 39, 40}, {30, 31, 36, 37, 42, 43}, {32, 33, 38, 39, 44, 35},
    {34, 35, 40, 41, 46, 47}, {37, 38, 43, 44, 48, 49} , {39, 40, 45, 46, 50, 51},
    {44, 45, 49, 50, 52, 53} } };

const array<vector<int>, 54> criteriaGoals
    { {{0, 1}, {0, 2}, {3, 5}, {1, 3, 6}, {2, 4, 7}, {4, 8}, {9, 12}, {5, 9, 13},
       {6, 10, 14}, {7, 10, 15}, {8, 11, 16}, {11, 17}, {12, 20}, {13, 18, 21},
       {14, 18, 22}, {15, 19, 23}, {16, 19, 24}, {17, 25}, {20, 26, 29},
       {21, 26, 30}, {22, 27, 31}, {23, 27, 32}, {24, 28, 33}, {25, 28, 34},
       {29, 37}, {30, 35, 38}, {31, 35, 39}, {32, 36, 40}, {33, 36, 41},
       {34, 42}, {37, 43, 46}, {38, 43, 47}, {39, 44, 48}, {40, 44, 49},
       {41, 45, 50}, {42, 45, 51}, {46, 54}, {47, 52, 55}, {48, 52, 56},
       {49, 53, 57}, {50, 53, 58}, {51, 59}, {54, 60}, {55, 60, 63}, {56, 61, 64},
       {57, 61, 65}, {58, 62, 66}, {59, 62}, {63, 67}, {64, 67, 69}, {65, 68, 70},
       {66, 68}, {69, 71}, {70, 71}}};

const array<pair<int, int>, 72> goalCriteria { {
    {0, 1}, {0, 3}, {1, 4}, {2, 3}, {4, 5}, {2, 7}, {3, 8}, {4, 9},
    {5, 10}, {6, 7}, {8, 9}, {10, 11}, {6, 12}, {7, 13}, {8, 14},
    {9, 15}, {10, 16}, {11, 17}, {13, 14}, {15, 16}, {12, 18},
    {13, 19}, {14, 20}, {15, 21}, {16, 22}, {17, 23}, {18, 19},
    {20, 21}, {22, 23}, {18, 24}, {19, 25}, {20, 26}, {21, 27},
    {22, 28}, {23, 29}, {25, 26}, {27, 28}, {24, 30}, {25, 31},
    {26, 32}, {27, 33}, {28, 34}, {29, 35}, {30, 31}, {32, 33},
    {34, 35}, {30, 36}, {31, 37}, {32, 38}, {33, 39}, {34, 40},
    {35, 41}, {37, 38}, {39, 40}, {36, 42}, {37, 43}, {38, 44},
    {39, 45}, {40, 46}, {41, 47}, {42, 43}, {44, 45}, {46, 47},
    {43, 48}, {44, 49}, {45, 50}, {46, 51}, {48, 49}, {50, 51},
    {49, 52}, {50, 53}, {52, 53}
    } 
};

const array<string, 5> resourcesList {"caffeine", "lab", "lecture", "study", "tutorial"};


void Board::makeTiles(array<pair<int, int>, 19> tileInfo, TextDisplay *td) {

    for (int i = 0; i < 19; ++i) {
        vector<Observer *> v;
        for (auto it : tilesCriteria[i]) v.push_back(theCriteria[it].get());
        theTiles.push_back(make_unique<Tile> (v, i, tileInfo[i].first, tileInfo[i].second));
        theCriteria.back()->attach(td);
	td->initTile(i, tileInfo[i].first, tileInfo[i].second);
    }
}

Board::Board() {}

Board::Board(array<pair<int, int>, 19> tileInfo, default_random_engine *eng,
             TextDisplay *td)
    : whoseTurn {0}, rolled {false}, gooseIndex {-1}, eng{eng} {
        thePlayers.push_back(make_unique<Player> ("Blue", eng));
        thePlayers.push_back(make_unique<Player> ("Red", eng));
        thePlayers.push_back(make_unique<Player> ("Orange", eng));
        thePlayers.push_back(make_unique<Player> ("Yellow", eng));
    activePlayer = thePlayers[0].get();
    for (int i = 0; i < 72; ++i) {
        theGoals.push_back(make_unique<Goal> (i));
        theGoals.back()->attach(td);
    }
    for (int i = 0; i < 54; ++i) {
        vector<Goal *> v;
        for (auto &it : criteriaGoals[i]) v.push_back(theGoals[it].get());
        theCriteria.push_back(make_unique<NoCriteria> (i, v));
        theCriteria.back()->attach(td);
    }
    for (int i = 0; i < 72; ++i) {
        theGoals[i]->attach(td);
        theGoals[i]->attachCriteria(theCriteria[goalCriteria[i].first].get());
        theGoals[i]->attachCriteria(theCriteria[goalCriteria[i].second].get());
    }
    makeTiles(tileInfo, td);
}

// helper for loading
pair<bool, int> exists (int find, array<vector<int>, 4> arr) {
    for (int i = 0; i < 4; ++i) {
        for (auto it : arr[i]) {
            if (find == it) return {true, i};
        }
     }
     return {false, 0};
}

Board::Board(int curTurn,
	array<pair<int, int>, 19> tileInfo,
	array<array<int, 5>, 4> playerResources,
	array<vector<int>, 4> playerGoals,
	array<vector<pair<int, int>>, 4> playerCriteria,
    	default_random_engine *eng,
    	int geese, 
	TextDisplay *td)
    : whoseTurn {curTurn}, rolled {false}, gooseIndex {geese}, eng{eng} {
    // players
    thePlayers.push_back(make_unique<Player> 
                    ("Blue", ResourceArray {playerResources[0]}, eng));
    thePlayers.push_back(make_unique<Player> 
                    ("Red", ResourceArray {playerResources[1]}, eng));
    thePlayers.push_back(make_unique<Player> 
                    ("Orange", ResourceArray {playerResources[2]}, eng));
    thePlayers.push_back(make_unique<Player> 
                    ("Yellow", ResourceArray {playerResources[3]}, eng));
    // setting active player
    activePlayer = thePlayers[whoseTurn].get();
    // setting goals
    for (int i = 0; i < 72; ++i) {
        pair <bool, int> check = exists(i, playerGoals);
        if (check.first) {
            theGoals.push_back(
            make_unique<Goal> (i, thePlayers[check.second].get()));
            thePlayers[check.second]->addGoal(i);
        }
        else theGoals.push_back(make_unique<Goal> (i));
        theGoals[i]->attach(td);
    }
    for (int i = 0; i < 54; ++i) {
        vector<Goal *> v;
        for (auto &it : criteriaGoals[i]) v.push_back(theGoals[it].get());
        theCriteria.push_back(make_unique<NoCriteria> (i, v));
        theCriteria.back()->attach(td);
    }
    for (int i = 0; i < 72; ++i) {
        theGoals[i]->attachCriteria(theCriteria[goalCriteria[i].first].get());
        theGoals[i]->attachCriteria(theCriteria[goalCriteria[i].second].get());
    }
    for (int i = 0; i < 4; ++i) {
        for (auto &it : playerCriteria[i]) {
            for (int j = 0; j < it.second; ++j) upgradeCriteriaFromSave(it.first, thePlayers[i].get());
        }
    }
    makeTiles(tileInfo, td);
    for(auto &it : theCriteria) if(!it->isBuilt()) it->notifyObservers();
    for(auto &it : theGoals) it->notifyObservers();
}


void Board::upgradeCriteriaFromSave(int index, Player *newOwner) {
    theCriteria[index] = theCriteria[index]->upgradeCriteria(newOwner, true, true);
}


string Board::getWhoseTurn() const { // Blue, Red, Orange, Yellow
    if (whoseTurn == 0) return "Blue";
    else if (whoseTurn == 1) return "Red";
    else if (whoseTurn == 2) return "Orange";
    else if (whoseTurn == 3) return "Yellow";
    else return "";
}


int Board::getGoose() const {
    return gooseIndex;
}


array<int, 4> Board::moveGoose(int newLoc) {
    gooseIndex = newLoc;
    array<int, 4> stealFromPlayer{};
    for (int i = 0; i < 6; ++i) {
        if (theCriteria[tilesCriteria[newLoc][i]]->isOwnedBy(activePlayer)) continue;
        else {
        for (int j = 0; j < 4; ++j) {
            if (theCriteria[tilesCriteria[newLoc][i]]->isOwnedBy(thePlayers[j].get())) {
                if (thePlayers[j]->getResources().sum() > 0) stealFromPlayer[j] = 1;
                }
            }
        }
    }
    return stealFromPlayer;
}


string Board::steal(int playerIndex) {
    string res = thePlayers[whoseTurn]->steal(thePlayers[playerIndex].get());
    return "Student " + activePlayer->print() + " steals " + res + 
            "from student " + thePlayers[playerIndex]->print() + "\n";
}


bool Board::gameOver() {
    for (int i = 0; i < 4; ++i) {
        if (thePlayers[i]->calculateScore() == 10) {
		winnerIndex = i;
		return true;
	}
    }
    return false;
}

string Board::whoWon() const {
	return thePlayers.at(winnerIndex)->print();
}


void Board::load(int val) {
    activePlayer->loaded(val);
}


void Board::fair() {
    activePlayer->fair();
}


// throw exception
string Board::roll() {
    const int val = activePlayer->roll();
    rolled = true;
    string excptStr;
    ostringstream oss;
    oss << val;
    array<ResourceArray, 4> beforeGained;
    if (val == 7) {
        for (auto &it : thePlayers) excptStr += it->goose();
        throw excptStr;
    }
    for (int i = 0; i < 4; ++i) {
        beforeGained[i] = thePlayers[i]->getResources();
    }
    for (auto &it : theTiles) {
        if (it->valueEqual(val) && it->getID() != gooseIndex) it->notifyObservers();
    }
    return oss.str() + " was rolled.\n\n" + resourceGained(beforeGained);
}


std::string Board::getPlayerStatus() const {
    return thePlayers.at(0)->status()
    	+ "\n" + thePlayers.at(1)->status()
	+ "\n" + thePlayers.at(2)->status()
	+ "\n" + thePlayers.at(3)->status();
}


std::string Board::getCriteria() const {
    return activePlayer->getCriteria();    
}

// may throw exception!! have to catch in controller!!
void Board::achieve(int index) {
    theGoals[index]->buildGoal(activePlayer);
}


void Board::reattachCriteriaToTile(int index, Criteria *oldC) {
    for(int i = 0; i < 19; ++i) { // for each tile
        for(int j = 0; j < 6; ++j) { // for each criteria attached to tile
            if(tilesCriteria[i][j] == index) {
                theTiles[i]->detach(oldC);
                theTiles[i]->attach(theCriteria[index].get());
            }
        }
    }
}

// may throw exception!! have to catch in controller!!
void Board::buildCriteria(int index) {
    Criteria *oldC = theCriteria[index].get();
    theCriteria[index] = theCriteria[index]->upgradeCriteria(activePlayer);
    reattachCriteriaToTile(index, oldC);
}

void Board::beginningCrit(int index, int player, const bool overrideChecks) {
    Criteria *oldC = theCriteria[index].get();
    theCriteria[index] = theCriteria[index]->upgradeCriteria(thePlayers[player].get(), overrideChecks);
    reattachCriteriaToTile(index, oldC);
}

// may throw exception!! have to catch in controller!!
void Board::upgradeCriteria(int index) {
    Criteria *oldC = theCriteria[index].get();
    theCriteria[index] = theCriteria[index]->upgradeCriteria(activePlayer);
    reattachCriteriaToTile(index, oldC);
}


pair<string, pair<ResourceArray, ResourceArray>> Board::tradePrompt(int player, int giveResource, int takeResource) {
    array<int, 5> activeArr{} , otherArr{};
    activeArr[giveResource] = -1;
    activeArr[takeResource] = 1;
    otherArr[takeResource] = -1;
    otherArr[giveResource] = 1;
    ResourceArray activeRArr {activeArr};
    ResourceArray otherRArr {otherArr};
    if(!activePlayer->haveEnoughResources(activeRArr)) {
        throw(string {"The proposing player does not have enough resources"});
    }
    if(!thePlayers[player]->haveEnoughResources(otherRArr)) {
        throw(string {"The accepting player does not have enough resources"});
    }
    return {activePlayer->print() + " offers " + thePlayers[player]->print() + " one " + resourcesList[giveResource] +
            " for one " + resourcesList[takeResource] + ".\nDoes " + thePlayers[player]->print() + " accept this offer? (yes/no)\n", 
            {activeRArr, otherRArr}};
}

void Board::trade(int Player, pair<ResourceArray, ResourceArray> takeGive) {
    activePlayer->trade(takeGive.first);
    thePlayers[Player]->trade(takeGive.second);
}

void Board::next() {
    whoseTurn = (whoseTurn + 1) % 4;
    activePlayer = thePlayers[whoseTurn].get();
    rolled = false;
}


void Board::save(string file, bool rolledAlready) const {
    ofstream myFile (file);
    if (myFile.is_open()){
        if (rolled) {
            ostringstream oss;
	    if (rolledAlready) {
            	oss << (whoseTurn + 1) % 4;
	    }
	    else {
		oss << whoseTurn;
	    }
            myFile << oss.str() + "\n";
        } else {
            ostringstream oss;
            oss << whoseTurn;
            myFile << oss.str() + "\n";
        }
        for (auto &it : thePlayers) myFile << it->save() + "\n";
        for (auto &it : theTiles) myFile << it->savePrint() + " ";
        ostringstream goose;
        goose << gooseIndex;
        myFile << "\n" + goose.str() << endl;
        myFile.close();
    }
}

string Board::resourceGained(array<ResourceArray, 4> before) const {
    ResourceArray change;
    string retStr;
    bool gained = false;
    for (int i = 0; i < 4; ++i) { 
        change = thePlayers[i]->getResources() + (before[i] * -1);
        if (change.sum() <= 0) continue;
        else {
            gained = true;
            retStr += "Student " + thePlayers[i]->print() + " gained:\n";
            for (int j = 0; j < 5; ++j) {
                if (change[j] > 0) {
                    ostringstream oss;
                    oss << change[j];
                    if (j == 0) retStr += oss.str() + " caffeines\n";
                    else if (j == 1) retStr += oss.str() + " labs\n";
                    else if (j == 2) retStr += oss.str() + " lectures\n";
                    else if (j == 3) retStr += oss.str() + " studies\n";
                    else retStr += oss.str() + " tutorials\n";
                }
            }
        }
    }
    if (gained) return retStr;
    else return "No students gained resources.\n";
}
