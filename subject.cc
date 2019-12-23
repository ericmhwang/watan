#include "subject.h"
using namespace std;

void Subject::attach(Observer *ob) {
    observers.push_back(ob);
}

void Subject::detach(Observer *ob) {
    auto it = observers.begin();
    for(; it != observers.end(); ++it) if(*it == ob) break;
    observers.erase(it);
}
