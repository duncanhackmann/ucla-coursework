//
//  VacationAccount.cpp
//  Project 6
//
//  Created by Duncan Hackmann on 3/19/23.
//

#include "VacationAccount.h"

VacationAccount::VacationAccount(int EmployeeID) {
    mID = EmployeeID;
    mBalance = 0;
}

double VacationAccount::getBalance() {
    return mBalance;
}

int VacationAccount::getID() {
    return mID;
}

bool VacationAccount::addVacationToAccount(BloodDonation donation) {
    if ((mID == donation.getID()) && (donation.getID() != -1) && (donation.getAge() != -1) && (donation.getWeight() != -1)) {
        mBalance += 4;
        return true;
    } else {
        return false;
    }
}
