#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>

class Observer;

class Subject {
  protected:
    std::vector<Observer *> observers;
  public:
    virtual void notifyObservers() = 0;
    void attach(Observer *ob);
    void detach(Observer *ob);
    virtual ~Subject() = default;
};

#endif
