#include "textdisplay.h"
#include "goal.h"
#include "criteria.h"
#include <map>
#include <sstream>
using namespace std;

const string resourceTypes[6] = {"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL", "NETFLIX"};
const string players[4] = {"B", "R", "O", "Y"};

string spaces(int numSpaces) {
    string retStr;
    for (int i = 0; i <= numSpaces; ++i) retStr += " ";
    return retStr;
}


// printLeft determines whether to print the leftmost slash. True will print it.
// printRight does the same for the rightmost backslash. True will print it.
string printTileTop(bool printLeft = 1, bool printRight  = 1 ) {
    return ((printLeft)?("/"):("")) + spaces(11) + ((printRight)?("\\"):(""));
}

// printLeft determines whether to print the leftmost backslash.
// printRight does the same for the rightmost slash. True will print it.
string printTileBottom(bool printLeft = 1, bool printRight  = 1 ) {
    return ((printLeft)?("\\"):("")) + spaces(11) + ((printRight)?("/"):(""));
}

string TextDisplay::printTileID(int index) const {
    string retStr;
    ostringstream oss;
    int id = myResources[index].id;
    oss << id;
    if (id < 10) retStr = " " + oss.str();
    else retStr = oss.str();
    return spaces(5) + retStr + spaces(4);
}

string TextDisplay::printTileVal(int index) const {
    string retStr;
    ostringstream oss;
    int val = myResources[index].value;
    oss << val;
    if (val==7) retStr = "  ";
    else if (val < 10) retStr = " " + oss.str();
    else retStr = oss.str();
    return spaces(6) + retStr + spaces(6);
}

string TextDisplay::printTileRes(int index) const {
    return "/" + spaces(5) + myResources[index].resource
        + spaces(9 - myResources[index].resource.length()) + "\\";
}

string TextDisplay::printGooseArea(int index) const {
    return "\\" + spaces(5) + ((index==goose)?("GEESE" + spaces(4)):(spaces(9))) + "/";
}

string TextDisplay::printCriteria(int index) const {
    return "|" + myCriteria[index] + "|";
}

string TextDisplay::printHorizGoal(int index) const {
    return "--" + myGoals[index] + "--";
}

void TextDisplay::init(array<pair<int, int>, 19> board) {
    for (int i = 0; i < 54; ++i) {
        ostringstream oss;
        oss << i;
        if (i < 10) myCriteria[i] = " " + oss.str();
        else myCriteria[i] = oss.str();
    }
    for (int i = 0; i < 72; ++i) {
        ostringstream oss;
        oss << i;
        if (i < 10)  myGoals[i] = " " + oss.str();
        else myGoals[i] = oss.str();
    }
    for (int i = 0; i < 19; ++i) {
        initTile(i, board[i].first, board[i].second);
    }
}

// goose is -1 in the beginning
TextDisplay::TextDisplay(array<pair<int, int>, 19> board): goose {-1} {
    init(board);
}

TextDisplay::TextDisplay(array<std::pair<int, int>, 19> board,
  array<vector<int>, 4> playerGoals, array<vector<pair<int, int>>, 4> playerCriteria, 
  int goose): goose {goose} {
    init(board);
    for (int i = 0; i < 4; ++i) {
        for (auto &it : playerGoals[i]) myGoals[it] = players[i] + "A";
        for (auto &it : playerCriteria[i]) {
            string crit = players[i];
            if (it.second == 1) crit += "A";
            else if (it.second == 2) crit += "M";
            else if (it.second == 3) crit += "E";
            myCriteria[it.first] = crit;
        }
    }
}

void TextDisplay::notify(Goal &fromGoal) {
    myGoals[fromGoal.getID()] = fromGoal.print();
}

void TextDisplay::notify(Criteria &fromCriteria) {
    myCriteria[fromCriteria.getID()] = fromCriteria.print();
}

void TextDisplay::initTile(int id, int resource, int val) {
    myResources[id] = printTile{id, resourceTypes[resource], val};
}

void TextDisplay::moveGoose(int index) {
    goose = index;
}

ostream & operator<<(ostream &out, const TextDisplay &td) {
   
    // line 1
    out << spaces(35);
    out << td.printCriteria(0) + td.printHorizGoal(0) + td.printCriteria(1) << endl;

    // line 2
    out << spaces(35);
    out << printTileTop() << endl;

    // line 3
    out << spaces(33);
    out << td.myGoals[1] << td.printTileID(0) << td.myGoals[2] << endl;

    // line 4
    out << spaces(33);
    out << td.printTileRes(0) << endl;

    // line 5
    out << spaces(20);
    out << td.printCriteria(2) + td.printHorizGoal(3) + td.printCriteria(3)
        + td.printTileVal(0) + td.printCriteria(4)
        + td.printHorizGoal(4) + td.printCriteria(5)<< endl;

    // line 6
    out << spaces(20);
    out << printTileTop(1, 0) + td.printGooseArea(0) + printTileTop(0,1) << endl;

    // line 7
    out << spaces(18);
    out << td.myGoals[5] + td.printTileID(1) + td.myGoals[6] + spaces(12)
        + td.myGoals[7] + td.printTileID(2) + td.myGoals[8] << endl;

    // line 8
    out << spaces(18);
    out << td.printTileRes(1) + printTileBottom(0, 0) + td.printTileRes(2) << endl;

    // line 9
    out << spaces(5);
    out << td.printCriteria(6) + td.printHorizGoal(9) + td.printCriteria(7)
        + td.printTileVal(1) + td.printCriteria(8) + td.printHorizGoal(10)
        + td.printCriteria(9) + td.printTileVal(2) + td.printCriteria(10)
        + td.printHorizGoal(11) + td.printCriteria(11) << endl;

    // line 10
    out << spaces(5);
    out << printTileTop(1, 0) + td.printGooseArea(1) + printTileTop(0, 0)
        + td.printGooseArea(2) + printTileTop(0, 1) << endl;

    // line 11
    out << spaces(3);
    out << td.myGoals[12] + td.printTileID(3) + td.myGoals[13] + spaces(12)
        + td.myGoals[14] + td.printTileID(4) + td.myGoals[15] + spaces(12)
        + td.myGoals[16] + td.printTileID(5) + td.myGoals[17] << endl;

    // line 12
    out << spaces(3);
    out << td.printTileRes(3) + printTileBottom(0, 0) + td.printTileRes(4)
        + printTileBottom(0, 0) + td.printTileRes(5) << endl;

    // line 13
    out << " " + td.printCriteria(12) + td.printTileVal(3) + td.printCriteria(13)
        + td.printHorizGoal(18) + td.printCriteria(14) + td.printTileVal(4)
        + td.printCriteria(15) + td.printHorizGoal(19) + td.printCriteria(16)
        + td.printTileVal(5) + td.printCriteria(17) << endl;

    // line 14
    out << spaces(3);
    out << td.printGooseArea(3) + printTileTop(0, 0) + td.printGooseArea(4)
        +  printTileTop(0, 0) + td.printGooseArea(5) << endl;

    // line 15
    out << spaces(3);
    out << td.myGoals[20] + spaces(12) + td.myGoals[21] + td.printTileID(6)
        + td.myGoals[22] + spaces(12) + td.myGoals[23] + td.printTileID(7)
        + td.myGoals[24] + spaces(12) + td.myGoals[25] << endl;


    // line 16
    out << spaces(5);
    out << printTileBottom(1, 0) + td.printTileRes(6) + printTileBottom(0, 0)
        + td.printTileRes(7) + printTileBottom(0, 1) << endl;

    // line 17
    out << spaces(5);
    out << td.printCriteria(18) + td.printHorizGoal(26) + td.printCriteria(19)
        + td.printTileVal(6) + td.printCriteria(20) + td.printHorizGoal(27)
        + td.printCriteria(21) + td.printTileVal(7) + td.printCriteria(22)
        + td.printHorizGoal(28) + td.printCriteria(23) << endl;

    // line 18
    out << spaces(5);
    out << printTileTop(1, 0) + td.printGooseArea(6) + printTileTop(0, 0)
        + td.printGooseArea(7) + printTileTop(0, 1) << endl;

    // line 19
    out << spaces(3);
    out << td.myGoals[29] + td.printTileID(8) + td.myGoals[30] + spaces(12)
        + td.myGoals[31] + td.printTileID(9) + td.myGoals[32] + spaces(12)
        + td.myGoals[33] + td.printTileID(10) + td.myGoals[34] << endl;

    // line 20
    out << spaces(3);
    out << td.printTileRes(8) + printTileBottom(0, 0) + td.printTileRes(9)
        + printTileBottom(0, 0) + td.printTileRes(10) << endl;

    // line 21
    out << " " + td.printCriteria(24) + td.printTileVal(8) + td.printCriteria(25)
        + td.printHorizGoal(35) + td.printCriteria(26) + td.printTileVal(9)
        + td.printCriteria(27) + td.printHorizGoal(36) + td.printCriteria(28)
        + td.printTileVal(10) + td.printCriteria(29) << endl;

    // line 22
    out << spaces(3);
    out << td.printGooseArea(8) + printTileTop(0, 0) + td.printGooseArea(9)
        +  printTileTop(0, 0) + td.printGooseArea(10) << endl;

    // line 23
    out << spaces(3);
    out << td.myGoals[37] + spaces(12) + td.myGoals[38] + td.printTileID(11)
        + td.myGoals[39] + spaces(12) + td.myGoals[40] + td.printTileID(12)
        + td.myGoals[41] + spaces(12) + td.myGoals[42] << endl;

    // line 24
    out << spaces(5);
    out << printTileBottom(1, 0) + td.printTileRes(11) + printTileBottom(0, 0)
        + td.printTileRes(12) + printTileBottom(0, 1) << endl;

    // line 25
    out << spaces(5);
    out << td.printCriteria(30) + td.printHorizGoal(43) + td.printCriteria(31)
        + td.printTileVal(11) + td.printCriteria(32) + td.printHorizGoal(44)
        + td.printCriteria(33) + td.printTileVal(12) + td.printCriteria(34)
        + td.printHorizGoal(45) + td.printCriteria(35) << endl;

    // line 26
    out << spaces(5);
    out << printTileTop(1, 0) + td.printGooseArea(11) + printTileTop(0, 0)
        + td.printGooseArea(12) + printTileTop(0, 1) << endl;

    // line 27
    out << spaces(3);
    out << td.myGoals[46] + td.printTileID(13) + td.myGoals[47] + spaces(12)
        + td.myGoals[48] + td.printTileID(14) + td.myGoals[49] + spaces(12)
        + td.myGoals[50] + td.printTileID(15) + td.myGoals[51] << endl;

    // line 28
    out << spaces(3);
    out << td.printTileRes(13) + printTileBottom(0, 0) + td.printTileRes(14)
        + printTileBottom(0, 0) + td.printTileRes(15) << endl;

    // line 29 
    out << " " + td.printCriteria(36) + td.printTileVal(13) + td.printCriteria(37)
        + td.printHorizGoal(52) + td.printCriteria(38) + td.printTileVal(14)
        + td.printCriteria(39) + td.printHorizGoal(53) + td.printCriteria(40)
        + td.printTileVal(15) + td.printCriteria(41) << endl;

    // line 30
    out << spaces(3);
    out << td.printGooseArea(13) + printTileTop(0, 0) + td.printGooseArea(14)
        +  printTileTop(0, 0) + td.printGooseArea(15) << endl;

    // line 31
    out << spaces(3);
    out << td.myGoals[54] + spaces(12) + td.myGoals[55] + td.printTileID(16)
        + td.myGoals[56] + spaces(12) + td.myGoals[57] + td.printTileID(17)
        + td.myGoals[58] + spaces(12) + td.myGoals[59] << endl;

    // line 32
    out << spaces(5);
    out << printTileBottom(1, 0) + td.printTileRes(16) + printTileBottom(0, 0)
        + td.printTileRes(17) + printTileBottom(0, 1) << endl;
    
    // line 33
    out << spaces(5);
    out << td.printCriteria(42) + td.printHorizGoal(60) + td.printCriteria(43)
        + td.printTileVal(16) + td.printCriteria(44) + td.printHorizGoal(61)
        + td.printCriteria(45) + td.printTileVal(17) + td.printCriteria(46)
        + td.printHorizGoal(62) + td.printCriteria(47) << endl;

    // line 34
    out << spaces(18);
    out << td.printGooseArea(16) + printTileTop(0, 0) + td.printGooseArea(17);
    out << endl;

    // line 35
    out << spaces(18);
    out << td.myGoals[63] + spaces(12) + td.myGoals[64] + td.printTileID(18)
        + td.myGoals[65] + spaces(12) + td.myGoals[66] << endl;

    // line 36
    out << spaces(20);
    out << printTileBottom(1, 0) + td.printTileRes(18) + printTileBottom(0, 1);
    out << endl;

    // line 37
    out << spaces(20);
    out << td.printCriteria(48) + td.printHorizGoal(67) + td.printCriteria(49)
        + td.printTileVal(18) + td.printCriteria(50) + td.printHorizGoal(68)
        + td.printCriteria(51) << endl;

    // line 38
    out << spaces(33);
    out << td.printGooseArea(18) << endl;

    // line 39
    out << spaces(33);
    out << td.myGoals[69] + spaces(12) + td.myGoals[70] << endl;

    // line 40
    out << spaces(35);
    out << printTileBottom(1,1) << endl;

    // line 41
    out << spaces(35);
    out << td.printCriteria(52) + td.printHorizGoal(71) + td.printCriteria(53);
    out << endl;

    return out;
}
