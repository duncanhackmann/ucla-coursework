//
//  FrequentFlyerAccount.cpp
//  Project 5
//
//  Created by Duncan Hackmann on 3/5/23.
//

#include "FrequentFlyerAccount.h"
#include "PlaneFlight.h"

//constructor
FrequentFlyerAccount::FrequentFlyerAccount(string name) {
    mName = name;
    mBalance = 0;
}

//getters
double FrequentFlyerAccount::getBalance() {
    return mBalance;
}

string FrequentFlyerAccount::getName() {
    return mName;
}


bool FrequentFlyerAccount::addFlightToAccount(PlaneFlight flight) {
        if (flight.getName() == mName) {
            mBalance += flight.getMileage();
            return true;
        }
        return false;
    }


bool FrequentFlyerAccount::canEarnFreeFlight(double mileage) {
        return mBalance >= mileage;
    }

bool FrequentFlyerAccount::freeFlight(string from, string to, double mileage, PlaneFlight& flight) {
        if (canEarnFreeFlight(mileage)) {
            flight = PlaneFlight(mName, from, to, 0, mileage);
            mBalance -= mileage;
            return true;
        }
        return false;
    }
