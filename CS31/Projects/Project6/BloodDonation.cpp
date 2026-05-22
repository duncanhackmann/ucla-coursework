//
//  BloodDonation.cpp
//  Project 6
//
//  Created by Duncan Hackmann on 3/19/23.
//

#include "BloodDonation.h"

BloodDonation::BloodDonation(int EmployeeID, int EmployeeAge, double EmployeeWeight) {
        
    if (EmployeeID > 999999 || EmployeeID < 100000) {
        mID = -1;
    } else {
        mID = EmployeeID;
    }
    
    
    if (EmployeeAge > 65 || EmployeeAge < 21) {
        mAge = -1;
    } else {
        mAge = EmployeeAge;
    }

    if (EmployeeWeight < 101.00 || EmployeeWeight > 280.00) {
        mWeight = -1;
    } else {
        mWeight = EmployeeWeight;
    }
}


int BloodDonation::getID() {
    return mID;
}

void BloodDonation::setID(int EmployeeID) {
    if (EmployeeID > 999999 || EmployeeID < 100000) {
        mID = -1;
        
    } else {
        mID = EmployeeID;
    }
}


int BloodDonation::getAge() {
    return mAge;
}

void BloodDonation::setAge(int EmployeeAge) {
    if (EmployeeAge > 65 || EmployeeAge < 21) {
        mAge = -1;
    } else {
        mAge = EmployeeAge;
    }
}


double BloodDonation::getWeight() {
    return mWeight;
}

void BloodDonation::setWeight(double EmployeeWeight) {
    if (EmployeeWeight < 101.00 || EmployeeWeight > 280.00) {
        mWeight = -1;
    } else {
        mWeight = EmployeeWeight;
    }
}

