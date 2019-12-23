#include <utility>
#include <fstream>
#include <sstream>
#include <cassert> // for testing
#include "controller.h"
#include "board.h"
#include "textdisplay.h"

using namespace std;
// catch exceptions and output appropriate messages

Controller::Controller() {}

void Controller::setEngine(default_random_engine &eng) {
	this->eng = eng;
}

string Controller::whoseTurn() const {
	return theBoard->getWhoseTurn(); 
}

bool Controller::gameOver() {
	return theBoard->gameOver();	
}

string Controller::whoWon() const {
	return theBoard->whoWon();
}

// ********** Command-line Argument related methods **********

void Controller::newGame() {

	array<pair<int, int>, 19> arg;
	array<int, 6> remainingRes{ {3, 3, 4, 4, 4, 1} };
	array<int, 12> remainingVal{ {0, 1, 2, 2, 2, 2, 0, 2, 2, 2, 2, 1} };

	for (int i = 0; i < 19; ++i) {

		// random number generator for resources
		uniform_int_distribution<int> resDist(0, 5);
		int r{resDist(eng)};

		// If we have already assigned enough of that resource type
		while (remainingRes.at(r) == 0) {
			r = resDist(eng);
		}
		--remainingRes.at(r);
		
		int v;
		if (r == 5) {
			v = 7;
		} else {
			// random number generator for values
			uniform_int_distribution<int> valDist(1, 12);
			v = {valDist(eng)};

			// If we have already assigned enough of that number
			while (remainingVal.at(v - 1) == 0) {
				v = valDist(eng);
			}
			--remainingVal.at(v - 1);
		}

		pair<int, int> tileInfo{r, v};
		arg.at(i) = tileInfo;
	}

	// Ensure these are calling the move assignment operator
	textDisplay = make_unique<TextDisplay>(arg);
	theBoard = make_unique<Board>(arg, &eng, textDisplay.get());
}

void Controller::loadGame(const string file) {

	ifstream ifs{file};
	int curTurn;
	array<pair<int, int>, 19> board;
	array<array<int, 5>, 4> playerResources;
	array<vector<int>, 4> playerGoals;
	array<vector<pair<int, int>>, 4> playerCriteria;
	int geese;

	// assumes there are 7 lines in the saved file
	for (int i = 0; i < 7; ++i) {
		string line;
		getline(ifs, line);

		if (ifs.eof() && i == 6) {
			geese = -1;
			break;
		}
		istringstream iss{line};

		string input;
		while (iss >> input) {
			// <curTurn>
			if (i == 0) {
				curTurn = stoi(input);
			}
			// <studentData>
			else if (i >= 1 && i <= 4) {

				const int playerI{i - 1};

				// update playerResources
				array<int, 5> resources;
				for (int resI = 0; resI < 5; ++resI) {
					resources.at(resI) = stoi(input);
					iss >> input;
				}
				playerResources.at(playerI) = resources;

				assert(input == "g");
				iss >> input;

				// update playerGoals
				vector<int> goals;
				while (input != "c") {
					goals.emplace_back(stoi(input));	
					iss >> input;
				}
				playerGoals.at(playerI) = goals;

				assert(input == "c");

				// update playerCriteria
				vector<pair<int, int>> criteria;
				while (iss >> input) {
					int ind;
					iss >> ind;
					criteria.emplace_back(stoi(input), ind);
				}
				playerCriteria.at(playerI) = criteria;
			}
			// <board>
			else if (i == 5) {

				int index{0};
				while (true) {
					int r{stoi(input)};
					iss >> input;
					int v{stoi(input)};

					pair<int, int> tileInfo{r, v};
					board.at(index++) = tileInfo;

					if (!(iss >> input)) {
						break;
					}
				}
			}
			// <geese>
			else {
				geese = stoi(input);
			}
		}
	}

	textDisplay.reset(new TextDisplay(board, playerGoals, playerCriteria,geese));	
	theBoard.reset(new Board(curTurn, board, playerResources, playerGoals, playerCriteria, &eng, geese, textDisplay.get()));
}

void Controller::loadBoard(const string file) {

	ifstream ifs{file};
	array<pair<int, int>, 19> arg;
	int index{0};

	// assumes there are 7 lines in the saved file
	for (int i = 0; i < 7; ++i) {
		string line;
		getline(ifs, line);

		// only care about the Board setup (6th line)
		if (i == 5) {
			istringstream iss{line};
			int r;
			while(iss >> r) {

				int v;
				iss >> v;
				pair<int, int> tileInfo{r, v};
				arg.at(index++) = tileInfo;
			}
		}
	}

	// we are loading just the board (no other game state or player-related info)
	// so we can call the "new game" ctors
	textDisplay = make_unique<TextDisplay>(arg);
	theBoard = make_unique<Board>(arg, &eng, textDisplay.get());
}


// ********** Beginning-of-turn methods **********

void Controller::load(const int x) {

	theBoard->load(x);
}

void Controller::fair() {
	theBoard->fair();
}

void Controller::roll() {

	try {
		cout << theBoard->roll(); 
	}
	catch (string &err) { // if 7 was rolled
		cout << "7 was rolled.";
		cout << err << endl;
		
		// Move the geese
		int newIndex;
		while(true) {
			cout << "Choose where to place the GEESE." << endl;
			cout << ">";
			string input;
			cin >> input;
			if (cin.eof()) {
				throw (string{"EOF encountered while moving GEESE :(  Game saved without moving GEESE."});
			}

			istringstream iss{input};
			iss >> newIndex;
			if (iss && newIndex >= 0 && newIndex <= 18 && newIndex != theBoard->getGoose()) {
				break;
			}
			else {
				cout << "Invalid location." << endl;
			}
		}
		array<int,4> availSteal{theBoard->moveGoose(newIndex)};
		textDisplay->moveGoose(newIndex);
		string outputStr = "Student " + theBoard->getWhoseTurn() + " can steal from:";
		// Steal (if possible)
		bool possible = false;

		for (int i = 0; i < 4; ++i) {
			if (availSteal[i] == 1) {
				possible = true;
				outputStr += "\n"+colourToIndex[i];
			}
		}
		
		if (possible) {
			cout << outputStr + "\n" + "Choose a student to steal from\n";
			int playerI = 0;
			while(possible) {
				cout << ">";
				string colour;
				cin >> colour;
				if (colour == "Blue" && availSteal[0] == 1) {
					playerI = 0;
					break;
				} else if (colour == "Red" && availSteal[1] == 1) {
					playerI = 1;
					break;
				} else if(colour == "Orange" && availSteal[2] == 1) {
					playerI = 2;
					break;
				} else if (colour == "Yellow" && availSteal[3] == 1) {
					playerI = 3;
					break;
				} else {
					cout << "Invalid student colour. Please select a valid student colour." << endl;
				}
			}
			cout << theBoard->steal(playerI);
		} else {
			cout << theBoard->getWhoseTurn() + " has no player to steal from" << endl;
		}
	}
}


// ********** End-of-turn methods **********

void Controller::board() const {
	cout << *textDisplay << endl;	
}

void Controller::status() const {
	cout << theBoard->getPlayerStatus() << endl;
}

void Controller::criteria() const {
	cout << theBoard->getCriteria() << endl;
}

void Controller::achieve(const int index) {
	try {
		theBoard->achieve(index);
	}
	catch (string &err) {
		cout << err << endl;
	}
}

void Controller::complete(const int index, const int player, const bool overrideChecks) {
    if (player < 0) try {
        theBoard->buildCriteria(index);
        }
        catch(string &str) {
            cout << str << endl;
        }
    else theBoard->beginningCrit(index, player);
}

void Controller::improve(const int index) {
	try {
		theBoard->upgradeCriteria(index);
	}
	catch (string &err) {
		cout << err << endl;
	}
}

void Controller::trade(const int toPlayer, const int give, const int take) {
	try {
		pair<string, pair<ResourceArray, ResourceArray>> output = theBoard->tradePrompt(toPlayer, give, take);
		cout << output.first << ">";
		string response;
		cin >> response;
		if (response == "yes") { 
			theBoard->trade(toPlayer, output.second);
			cout << "Trade Successful.\n";
		}
		else if (response == "no") cout << "Trade declined." << endl;
		else cout << "Invalid input. Try trading again." << endl;
	}
	catch (string &err) {
		cout << err << endl;
	}
}

void Controller::next() {
	theBoard->next();
}

void Controller::save(const string file, const bool rolledAlready) const {

	theBoard->save(file, rolledAlready);
}

void Controller::help() const {
	cout << "Valid commands:" << endl
		<< "board" << endl
		<< "status" << endl
		<< "criteria" << endl
		<< "achieve <goal>" << endl
		<< "complete <criterion>" << endl
		<< "improve <criterion>" << endl
		<< "trade <colour> <give> <take>" << endl
		<< "next" << endl
		<< "save <file>" << endl
		<< "help" << endl;
}


