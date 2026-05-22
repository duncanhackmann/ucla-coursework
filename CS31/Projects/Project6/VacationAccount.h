//
//  VacationAccount.h
//  Project 6
//
//  Created by Duncan Hackmann on 3/19/23.
//

#ifndef VacationAccount_h
#define VacationAccount_h

#include <iostream>
#include "BloodDonation.h"

using namespace std;

class VacationAccount{
private:
    int mID;
    double mBalance;
    
public:
    VacationAccount(int EmployeeID);
    double getBalance();
    int getID();
    bool addVacationToAccount(BloodDonation donation);
};

#endif /* VacationAccount_h */
