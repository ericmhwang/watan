#include <random>
#include <array>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "controller.h"

using namespace std;

// potential improvement: incorrect user input handling

const int maxGoalIndex{71};
const int maxCriteriaIndex{53};
const vector<string> colours{"Blue", "Red", "Orange", "Yellow"}; // move to controller.h?
const string backup{"backup.sv"};
bool saveCalled{false}; 

void usage(const string arg, const string context = "Watan") {

	cerr << "Usage: " << arg << " is an invalid command line option for "
		<< context << "."  << endl;
}

void usagePair(const string arg, const string type) {

	cerr << "Usage: option " << arg << " expects another argument of type "
		<< type << " to follow it." << endl;
}

// potential improvement: return in Map format
// [0]: seed in string format, [1]: file if -load set, else "", [2]: file if -board set, else ""
array<string, 3> setGame(int argc, char *argv[]) {

	array<string, 3> setup{ {"", "", ""} };

	for (int i = 1; i < argc; ++i) {	
		string arg{argv[i]};

		if (arg == "-seed") {

			if (i + 1 >= argc) {
				usagePair(arg, "int");
				break;
			}
			istringstream iss{argv[++i]};

			int seed;
			if (iss >> seed) {
				setup[0] = to_string(seed);
			}
			else {
				usage(argv[i], arg);
			}
		}
		else if (arg == "-load") {

			if (i + 1 >= argc) {
				usagePair(arg, "string");
				break;
			}
			string file{argv[++i]};
			setup[1] = file;
		}
		else if (arg == "-board") {

			if (i + 1 >= argc) {
				usagePair(arg, "string");
				break;
			}
			string file{argv[++i]};
			setup[2] = file;
		}
		else {
			usage(arg);
		}
	}
	return setup;
}

//    - prompt students to complete assignments in order
//    - B,R,O,Y,Y,O,R,B
void beginGame(unique_ptr<Controller> &control) {

	bool reverse{false};

	for (int i = 0; i >= 0 && i < (int)colours.size(); reverse ? --i : ++i) {

		cout << "Student " << colours.at(i) << ", where do you want to "
			<< "complete an Assignment?" << endl << ">";

		int index;
		if (cin >> index) {
			if (index >= 0 && index <= maxCriteriaIndex) {
				try {
					control->complete(index, i, true); 
				}
				catch (string &err) {
					cout << err << endl;
					reverse ? ++i : --i;
					continue;
				}
			}
			else {
				cout << to_string(index) << " is not a valid "
					<< "intersection index. It must be "
					<< "between 0 and " << maxCriteriaIndex 
					<< "."  << endl;
				reverse ? ++i : --i;
				continue;
			}
		}
		else if (cin.eof()) {
			cout << "Calling eof now is invalid! Now the whole game setup is ruined :(" << endl;
			throw;
		}
		else {
			reverse ? ++i : --i;
			continue;
		}

		if (i == (int)colours.size() - 1 && !reverse) {
            		++i;
			reverse = true;
		}
	}
}

void beginTurn(unique_ptr<Controller> &control) {

	cout << endl << "*****************************************************" << endl;
	cout << "Student " << control->whoseTurn() << "'s turn." << endl;
	while(true) {
		cout << ">";
		string cmd;
		cin >> cmd;

		if (cin.eof()) {
			control->save(backup, false);
			saveCalled = true;
			break;
		}
		else if (cmd == "load") {

			int x;
			while (true) {
				cout << "Input a roll between 2 and 12:" << endl;
				cout << ">";
				cin >> x;
				if (x >= 2 && x <= 12) {
					break;
				}
				else {
					cout << "Invalid roll." << endl;
					cin.clear();
					cin.ignore();
				}
			}
			control->load(x);
			cout << "Using a loaded dice for " << to_string(x) << "." << endl;
		}
		else if (cmd == "fair") {
			control->fair();
			cout << "Using a fair dice." << endl;
		}
		else if (cmd == "roll") {
			try {
				control->roll();
			}
			catch (string &err) {
				cout << err << endl;
				control->save(backup);
				saveCalled = true;
				break;
			}
			break;
		}
		else {
			cout << cmd << " is not a valid beginning-of-turn "
				<< "command. It must be one of \"load\", "
				<< "\"fair\", or \"roll\"." << endl;
		}
	}
}

void endTurn(unique_ptr<Controller> &control) {
	
	cin.clear();
	cin.ignore();

	while(!control->gameOver()) {
		
		cout << endl << ">";
		string line;
		getline(cin, line);
		if (cin.eof()) {
			control->save(backup);
			saveCalled = true;
			break;
		}

		istringstream iss{line};
		string cmd;
		iss >> cmd;

		if (cmd == "board") {
			control->board();
		}
		else if (cmd == "status") {
			control->status();
			cout << "";
		}
		else if (cmd == "criteria") {
			control->criteria();
		}
		else if (cmd == "achieve") {
			int index;
			iss >> index;
			if (index >= 0 && index <= maxGoalIndex) {
				control->achieve(index);
			}
			else {
				cout << index << " is not a valid goal index." << endl;
			}
		}
		else if (cmd == "complete") {
			int index;
			iss >> index;
            		control->complete(index);
     	} else if (cmd == "improve") {
			int index;
			iss >> index;
			control->improve(index);
		} else if (cmd == "trade") {
			int colour, give, take;
			iss >> colour;
			iss >> give;
			iss >> take;
			if (give == take) cout << "The resources you wish to trade must be different" << endl;
			else control->trade(colour, give, take);

			cin.ignore();
			cin.clear();
		}
		else if (cmd == "next") {
			control->next();
			break;
		}
		else if (cmd == "save") {
			string file;
			iss >> file;
			if (file == "") {
				cout << "Please input a valid file name." << endl;
			}
			else {
				control->save(file);
			}
		}
		else if (cmd == "help") {
			control->help();
		}
		else {
			cout << cmd << " is not a valid end-of-turn "
				<< "command. Type \"help\" for a list of valid "
				<< "commands."  << endl;
		}
	}
}

bool endGame(unique_ptr<Controller> &control) {

	while(true) {
		cout << "Would you like to play again?" << endl;
		cout << ">";

		string ans;
		cin >> ans;
		if (cin.eof()) {
			control->save(backup);
			saveCalled = true;
			break;
		}
		else if (ans == "yes") {
			return false;	
		}
		else if (ans == "no") {
			return true;
		}
		else {
			cout << ans << " is not a valid answer. Type \"yes\" "
				<< "or \"no\"." << endl;
		}
	}
	return true;
}

int main(int argc, char *argv[]) {

	// 1. Handle command-line arguments
	array<string, 3> setup{setGame(argc, argv)};

	// 2. Setup the controller according to the setup specs from 1.
	unique_ptr<Controller> control{new Controller};

	// set seed
	if (setup.at(0) != "") {

		default_random_engine eng{static_cast<unsigned int>(stoi(setup.at(0)))};
		control->setEngine(eng);
	}

	// load game
	if (setup.at(1) != "") {

		control->loadGame(setup.at(1));
	} 
	// load board
	else if (setup.at(2) != "") {

		control->loadBoard(setup.at(2));
	} 
	else {
       		control->newGame();
   	}


	// 3. (if not loding a saved GAME) Prompt each player to do initial setup of achievements
	if (setup.at(1) == "") {
		beginGame(control);
	}

	// 4. Print board
	control->board();

	// ***** This marks the end of our "initial setup" for the game *****
	// From now on, the user can save the game

	while(!(control->gameOver())) {

		if (saveCalled) {
			break;
		}
		beginTurn(control);
		
		if (saveCalled) {
			break;
		}
		endTurn(control);
	}

	if (!saveCalled) {
		string winner{control->whoWon()};
		cout << endl << "(づ｡◕‿‿◕｡)づ" << endl
			<< winner << " WON!!!" << endl;
	}

	// Play again
	if(!saveCalled && !endGame(control)) {
		main(argc, argv);
	}
}
