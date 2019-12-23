#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <random>
#include <string>
#include <memory>
#include <iostream>
#include <array>
#include "board.h"
#include "textdisplay.h"

class Controller {
	
	const int winPoint{10};
	const std::array<std::string, 4> colourToIndex = {
		{"Blue","Red", "Orange", "Yellow"}};
	std::unique_ptr<TextDisplay> textDisplay; // using default ctor... overwritten later
	std::unique_ptr<Board> theBoard;	// using default ctor... overwritten later
	std::default_random_engine eng;

	public:
	Controller();
	// ~Controller(); Default one should suffice

	void setEngine(std::default_random_engine &eng); // called iff seed is set

	// Accessor methods
	std::string whoseTurn() const; // return "Blue", "Yellow", etc...
	bool gameOver();
	std::string whoWon() const;

	// Command-line Argument related methods
	void newGame();
	void loadGame(const std::string file);
	void loadBoard(const std::string file);
	
	// Beginning-of-turn methods
	void load(const int x);
	void fair();
	void roll();

	// End-of-turn methods
	void board() const;
	void status() const;
	void criteria() const;
	void achieve(const int index);
	void complete(const int index, const int player = -1, const bool overrideChecks = false);
	void improve(const int index);
	void trade(const int player, const int giveResource, const int takeResource);
	void next();
	void save(const std::string file, const bool rolledAlready = true) const;
	void help() const;

	// either this or board()
	//friend std::ostream<<(std::ostream &out, const Controller &c);
};

#endif
