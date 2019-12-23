#ifndef _TEXT_DISPLAY_H_
#define _TEXT_DISPLAY_H_
#include <string>
#include <array>
#include <vector>
#include <iostream>
#include "observer.h"


struct printTile {
    int id;
    std::string resource;
    int value;
};

class TextDisplay : public Observer {
	// criteria list as output format
    std::array<std::string, 54> myCriteria;
	// goal list as output format
    std::array<std::string, 72> myGoals;
	// tile data for initialization
    std::array<printTile, 19> myResources;
	// goose location: index between 0 - 18
    int goose;
	void init(std::array<std::pair<int, int>, 19> board);
	std::string printTileID(int index) const;
	std::string printTileVal(int index) const;
	std::string printTileRes(int index) const;
    std::string printGooseArea(int index) const;
    std::string printCriteria(int index) const;
    std::string printHorizGoal(int index) const;

  public:
	// Constructors
	TextDisplay() {}
	// New game display constructor
	TextDisplay(std::array<std::pair<int, int>, 19> board);
	// load game display constructor; takes in board state, player data and geese location
	TextDisplay(std::array<std::pair<int, int>, 19> board,
				std::array<std::vector<int>, 4> playerGoals,
				std::array<std::vector<std::pair<int, int>>, 4> playerCriteria,
				int goose);
	void notify(Goal &fromGoal) override;
	void notify(Criteria &fromCriteria) override;
    // at the same time we init BOARD TILES call initTile
    void initTile(int id, int resource, int val); 
    void moveGoose(int index);
	friend std::ostream & operator<<(std::ostream &out, const TextDisplay &td);
};

#endif
