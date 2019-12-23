#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>
#include <vector>
#include <memory>
#include <random>
#include "tile.h"
#include "player.h"
#include "criteria.h"
#include "goal.h"
#include "resourceArray.h"

class TextDisplay;

class Board {

	const int winPoint{10};
	std::vector<std::unique_ptr<Tile>> theTiles;
	std::vector<std::unique_ptr<Player>> thePlayers;
	std::vector<std::unique_ptr<Criteria>> theCriteria;
	std::vector<std::unique_ptr<Goal>> theGoals;
 	Player *activePlayer;
	
	int whoseTurn;
	// used when saving the game to see if roll was issued
	bool rolled;
	int gooseIndex;
	int winnerIndex{-1};
    std::default_random_engine *eng;
	
	void upgradeCriteriaFromSave(int index, Player *newOwner); // imp
    void makeTiles(std::array<std::pair<int, int>, 19> tileInfo, TextDisplay *td);
    void reattachCriteriaToTile(int index, Criteria *oldC);
	std::string resourceGained(std::array<ResourceArray, 4> before) const;
  public:
	Board();
	// New game constructor
	Board(std::array<std::pair<int, int>, 19> tileInfo, std::default_random_engine *eng, TextDisplay *td);
	// Load game constructor; takes in board state, player data and goose location
	Board(int curTurn,
		std::array<std::pair<int, int>, 19> board,
		std::array<std::array<int, 5>, 4> playerResources,
		std::array<std::vector<int>, 4> playerGoals,
		std::array<std::vector<std::pair<int, int>>, 4> playerCriteria,
       	        std::default_random_engine *eng,
		int geese, 
		TextDisplay *td);

	std::string getWhoseTurn() const; // imp
	// returns location of the goose
	int getGoose() const; // imp
	std::array<int, 4> moveGoose(int newLoc); // imp
	std::string steal(int playerIndex); // imp
	bool gameOver(); // imp
	std::string whoWon() const;

	void load(int roll);
	void fair();
	std::string roll(); // imp

	std::string getPlayerStatus() const; // imp
	std::string getCriteria() const; 
	void achieve(int index); // imp
	void beginningCrit(int index, int player, const bool overrideChecks = true);
	void buildCriteria(int index); // imp
	void upgradeCriteria(int index); // imp
	std::pair<std::string, std::pair<ResourceArray, ResourceArray>> tradePrompt(int player, int giveResource, int takeResource); // imp
	void trade(int player, std::pair<ResourceArray, ResourceArray>takeGive);
	void next(); // imp
	void save(std::string file, const bool rolledAlready = true) const;
};

#endif
