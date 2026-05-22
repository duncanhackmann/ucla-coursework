//
//  event.cpp
//  Homework 3
//
//  Created by Duncan Hackmann on 5/10/23.
//

#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

class Event {
public:
    virtual ~Event() {}
    Event(const string& name) : mName(name) {}
    virtual bool isSport() const = 0;
    virtual string need() const = 0;
    virtual string name() const { return mName; }
private:
    string mName;
};

class BasketballGame : public Event {
public:
    BasketballGame(const string& name) : Event(name) {}
    bool isSport() const { return true; }
    string need() const { return "hoops"; }
    virtual ~BasketballGame() { cout << "Destroying the " + this->name() + " basketball game." << endl; }
};

class Concert : public Event {
public:
    Concert(const string& name, const string& genre) : Event(name), mGenre(genre) {}
    bool isSport() const { return false; }
    string need() const { return "a stage"; }
    virtual ~Concert() { cout << "Destroying the " + this->name() + " " + this->mGenre + " concert." << endl; }
private:
    string mGenre;
};

class HockeyGame : public Event {
public:
    HockeyGame(const string& name) : Event(name) {}
    bool isSport() const { return true; }
    string need() const { return "ice"; }
    virtual ~HockeyGame() { cout << "Destroying the " + this->name() + " concert." << endl; }
};
