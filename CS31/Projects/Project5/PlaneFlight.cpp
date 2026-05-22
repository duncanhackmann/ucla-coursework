//
//  PlaneFlight.cpp
//  Project 5
//
//  Created by Duncan Hackmann on 3/5/23.
//

#include "PlaneFlight.h"

//constructor
PlaneFlight::PlaneFlight(string passengerName, string fromCity, string toCity, double cost, double mileage) {
    if (cost >= 0) {
        mCost = cost;
    } else {
        mCost = -1;
    }
    
    if (fromCity != "" && fromCity != mToCity) {
        mFromCity = fromCity;
    }
    
    if (toCity != "" && toCity != mFromCity) {
        mToCity = toCity;
    }
    
    if (passengerName != "") {
        mName = passengerName;
    }
    
    if (mileage >= 0) {
        mMileage = mileage;
    } else {
        mMileage = -1;
    }
}

//getters
double PlaneFlight::getCost() {
    return mCost;
}

double PlaneFlight::getMileage() {
    return mMileage;
}

string PlaneFlight::getName() {
    return mName;
}

string PlaneFlight::getFromCity() {
    return mFromCity;
}

string PlaneFlight::getToCity() {
    return mToCity;
}


//setters
void PlaneFlight::setCost(double cost) {
    if (cost >= 0) {
        mCost = cost;
    } else {
        mCost = -1;
    }
}

void PlaneFlight::setMileage(double mileage) {
    if (mileage >= 0) {
        mMileage = mileage;
    } else {
        mMileage = -1;
    }
}

void PlaneFlight::setName(string name) {
    if (name != "") {
        mName = name;
    }
}

void PlaneFlight:: setFromCity(string from) {
    if (from != "" && from != mToCity) {
        mFromCity = from;
    }
}

void PlaneFlight:: setToCity(string to) {
    if (to != "" && to != mFromCity) {
        mToCity = to;
    }
}

