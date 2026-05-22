//
//  main.cpp
//  Project 6
//
//  Created by Duncan Hackmann on 3/14/23.
//

#include <iostream>
#include <string>
#include <cassert>
#include "BloodDonation.h"
#include "VacationAccount.h"
using namespace std;


int main(int argc, const char * argv[]) {
    

    
    BloodDonation doner1( 752401, 45, 102.56);
    BloodDonation doner2( 752402, 45, 102.56);
    cout << doner1.getAge() << endl;
    cout << doner1.getWeight()<< endl;
    
    cout << doner1.getID()<< endl;
    VacationAccount account(752402);
    cout << account.getBalance() << endl;
    
    account.addVacationToAccount(doner2);
    cout << account.getBalance() << endl;
    
    account.addVacationToAccount(doner2);
    cout << account.getBalance() << endl;
    return 0;
}
