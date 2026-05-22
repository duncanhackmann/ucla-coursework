//
//  main.cpp
//  Project2
//
//  Created by Duncan Hackmann on 1/23/23.
//

#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {
    
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(2);
    
    //declaring variables
    string state, month;
    double purchaseAmount, stateTax = 0, localTax = 0, levy = 0;
    int day, year;
    
    //prompts and collecting user input
    cout << "State Name: ";
    getline(cin, state);
    
    cout << "Purchase amount: ";
    cin >> purchaseAmount;
    cin.clear();
    cin.ignore(1000, '\n');
    
    cout << "Provide the month: ";
    getline(cin, month);
    
    cout << "Provide the day: ";
    cin >> day;
    cin.clear();
    cin.ignore(1000, '\n');
    
    cout << "Provide the year: ";
    cin >> year;
    cin.clear();
    cin.ignore(1000, '\n');
    
    //checks if the state is valid
    if (!((state == "Alabama") || (state == "Alaska") || (state == "Arizona") || (state == "Arkansas") || (state == "California") || (state == "Colorado") || (state == "Connecticut") || (state == "Delaware") || (state == "Florida") || (state == "Georgia") || (state == "Hawaii") || (state == "Idaho") || (state == "Illinois") || (state == "Indiana") || (state == "Iowa") || (state == "Kansas") || (state == "Kentucky") || (state == "Louisiana") || (state == "Maine") || (state == "Maryland") || (state == "Massachusetts") || (state == "Michigan") || (state == "Minnesota") || (state == "Mississippi") || (state == "Missouri") || (state == "Montana") || (state == "Nebraska") || (state == "Nevada") || (state == "New Hampshire") || (state == "New Jersey") || (state == "New Mexico") || (state == "New York") || (state == "North Carolina") || (state == "North Dakota") || (state == "Ohio") || (state == "Oklahoma") || (state == "Oregon") || (state == "Pennsylvania") || (state == "Rhode Island") || (state == "South Carolina") || (state == "South Dakota") || (state == "Tennessee") || (state == "Texas") || (state == "Utah") || (state == "Vermont") || (state == "Virginia") || (state == "Washington") || (state == "West Virginia") || (state == "Wisconsin") || (state == "Wyoming"))) {
        
        cout << "Invalid state!" << endl;
        
        //checks if the purchaseAmount is valid
        } else if (purchaseAmount <= 0) {
            cout << "Invalid amount!" << endl;
        
        //checks if the month is valid
        } else if (!((month == "January") || (month == "February") || (month == "March") || (month == "April") || (month == "May") || (month == "June") || (month == "July") || (month == "August") || (month == "September") || (month == "October") || (month == "November") || (month == "December"))) {
            
            cout << "Invalid month!" << endl;
            
        //checks if the day is valid
        } else if ((day < 1) || (day > 31)) {
            cout << "Invalid day!" << endl;
            
        //checks if the year is valid
        } else if ((year < 1) || (year > 2025)) {
            cout << "Invalid year!" << endl;
            
        } else {
            
            //checks if it is a tax free month and day
            if ((state == "Alabama" && month == "July" && (day >= 14 && day <=17)) || (state == "Arkansas" && month == "August" && (day == 6 || day <=7)) || (state == "Connecticut" && month == "August" && (day >= 21 && day <=27)) || (state == "Iowa" && month == "August" && (day == 5 || day == 6)) || (state == "Maryland" && month == "August" && (day >= 14 && day <= 20)) || (state == "Massachusetts" && month == "August" && (day == 14 || day == 15)) || (state == "Mississippi" && month == "July" && (day == 29 || day == 30)) || (state == "Missouri" && month == "April" && (day >= 19 && day <= 25)) || (state == "Nevada" && month == "August" && (day >= 5 && day <= 7)) || (state == "Ohio" && month == "August" && (day >= 5 && day <= 7)) || (state == "Oklahoma" && month == "August" && (day >= 6 && day <= 8)) || (state == "South Carolina" && month == "August" && (day >= 5 && day <= 7)) || (state == "Tennessee" && month == "July" && (day >= 29 && day <= 31)) || (state == "Texas" && month == "August" && (day >= 5 && day <= 7)) || (state == "West Virginia" && month == "August" && (day >= 5 && day <= 7)) || (state  == "Delaware") || (state == "New Hapshire") || (state == "Oregon") || (state == "Montana")) {
                
                cout << "Please pay a total of $" << purchaseAmount << endl;
            
            //calculates the amount of state and local tax and levy to add on to the purchase amount for each state
            } else {
                
                if (state == "Alabama") {
                    stateTax = purchaseAmount*0.04;
                    localTax = purchaseAmount * 0.0514;
                    
                    
                } else if (state == "Alaska") {
                    stateTax = purchaseAmount * 0.0143;
                    
                } else if (state == "Arizona") {
                    stateTax = purchaseAmount * 0.056;
                    localTax = purchaseAmount * 0.0277;
                    
                    
                } else if (state == "Arkansas") {
                    stateTax = purchaseAmount * 0.065;
                    localTax = purchaseAmount * 0.0293;
                    
                    
                } else if (state == "California") {
                    stateTax = purchaseAmount * 0.0725;
                    localTax = purchaseAmount * 0.0131;
                    levy = purchaseAmount * 0.01;
                    
                } else if (state == "Colorado") {
                    stateTax = purchaseAmount * 0.029;
                    localTax = purchaseAmount * 0.0473;
                    
                    
                } else if (state == "Connecticut") {
                    stateTax = purchaseAmount * 0.0635;
                    
                } else if (state == "Florida") {
                    stateTax = purchaseAmount * 0.06;
                    localTax = purchaseAmount * 0.0105;
                    
                    
                } else if (state == "Georgia") {
                    stateTax = purchaseAmount * 0.04;
                    localTax = purchaseAmount * 0.0329;
                    
                    
                } else if (state == "Hawaii") {
                    stateTax = purchaseAmount * 0.04;
                    localTax = purchaseAmount * 0.0041;
                    
                    
                } else if (state == "Idaho") {
                    stateTax = purchaseAmount * 0.06;
                    localTax = purchaseAmount * 0.0003;
                    
                    
                } else if (state == "Illinois") {
                    stateTax = purchaseAmount * 0.0625;
                    localTax = purchaseAmount * 0.0249;
                    
                    
                } else if (state == "Indiana") {
                    stateTax = purchaseAmount * 0.07;
                    
                } else if (state == "Iowa") {
                    stateTax = purchaseAmount * 0.06;
                    localTax = purchaseAmount * 0.0082;
                    
                    
                } else if (state == "Kansas") {
                    stateTax = purchaseAmount * 0.065;
                    localTax = purchaseAmount * 0.0217;
                    
                    
                } else if (state == "Kentucky") {
                    stateTax = purchaseAmount * 0.06;
                    
                } else if (state == "Louisiana") {
                    stateTax = purchaseAmount * 0.0445;
                    localTax = purchaseAmount * 0.05;
                    
                    
                } else if (state == "Maine") {
                    stateTax = purchaseAmount * 0.055;
                    
                } else if (state == "Maryland") {
                    stateTax = purchaseAmount * 0.06;
                    
                } else if (state == "Massachusetts") {
                    stateTax = purchaseAmount * 0.0625;
                    
                } else if (state == "Michigan") {
                    stateTax = purchaseAmount * 0.06;
                    
                } else if (state == "Minnesota") {
                    stateTax = purchaseAmount * 0.0688;
                    localTax = purchaseAmount * 0.0055;
                    
                    
                } else if (state == "Mississippi") {
                    stateTax = purchaseAmount * 0.07;
                    localTax = purchaseAmount * 0.0007;
                    
                    
                } else if (state == "Missouri") {
                    stateTax = purchaseAmount * 0.0423;
                    localTax = purchaseAmount * 0.039;
                    
                    
                } else if (state == "Nebraska") {
                    stateTax = purchaseAmount * 0.055;
                    localTax = purchaseAmount * 0.0135;
                    
                    
                } else if (state == "Nevada") {
                    stateTax = purchaseAmount * 0.0685;
                    localTax = purchaseAmount * 0.0129;
                    
                    
                } else if (state == "New Jersey") {
                    stateTax = purchaseAmount * 0.0663;
                    localTax = 0.9997;
                    
                    
                } else if (state == "New Mexico") {
                    stateTax = purchaseAmount * 0.0513;
                    localTax = purchaseAmount * 0.0269;
                    
                    
                } else if (state == "New York") {
                    stateTax = purchaseAmount * 0.04;
                    localTax = purchaseAmount * 0.0449;
                    
                    
                } else if (state == "North Carolina") {
                    stateTax = purchaseAmount * 0.0475;
                    localTax = purchaseAmount * 0.0222;
                    
                    
                } else if (state == "North Dakota") {
                    stateTax = purchaseAmount * 0.05;
                    localTax = purchaseAmount * 0.0185;
                    
                    
                } else if (state == "Ohio") {
                    stateTax = purchaseAmount * 0.0575;
                    localTax = purchaseAmount * 0.0142;
                    
                    
                } else if (state == "Oklahoma") {
                    stateTax = purchaseAmount * 0.045;
                    localTax = purchaseAmount * 0.0442;
                    
                    
                } else if (state == "Pennsylvania") {
                    stateTax = purchaseAmount * 0.06;
                    localTax = purchaseAmount * 0.0034;
                    
                    
                } else if (state == "Rhode Island") {
                    stateTax = purchaseAmount * 0.07;
                    
                } else if (state == "South Carolina") {
                    stateTax = purchaseAmount * 0.06;
                    localTax = purchaseAmount * 0.0143;
                    
                    
                } else if (state == "South Dakota") {
                    stateTax = purchaseAmount * 0.045;
                    localTax = purchaseAmount * 0.019;
                    
                    
                } else if (state == "Tennessee") {
                    stateTax = purchaseAmount * 0.07;
                    localTax = purchaseAmount * 0.0247;
                    
                    
                } else if (state == "Texas") {
                    stateTax = purchaseAmount * 0.0625;
                    localTax = purchaseAmount * 0.0194;
                    
                    
                } else if (state == "Utah") {
                    stateTax = purchaseAmount * 0.0595;
                    localTax = purchaseAmount * 0.0099;
                    levy = purchaseAmount * 0.0125;
                    
                } else if (state == "Vermont") {
                    stateTax = purchaseAmount * 0.06;
                    localTax = purchaseAmount * 0.0018;
                    
                    
                } else if (state == "Virginia") {
                    stateTax = purchaseAmount * 0.053;
                    localTax = purchaseAmount * 0.0035;
                    levy = purchaseAmount * 0.01;
                    
                } else if (state == "Washington") {
                    stateTax = purchaseAmount * 0.065;
                    localTax = purchaseAmount * 0.0267;
                    
                    
                } else if (state == "West Virginia") {
                    stateTax = purchaseAmount * 0.06;
                    localTax = purchaseAmount * 0.0039;
                    
                    
                } else if (state == "Wisconsin") {
                    stateTax = purchaseAmount * 0.05;
                    localTax = purchaseAmount * 0.0044;
                    
                    
                } else if (state == "Wyoming") {
                    stateTax = purchaseAmount * 0.04;
                    localTax = purchaseAmount * 0.0136;
                    
                    
                }
                //calculates and prints final purchase amount with state tax, local tax, and levy added on
                cout << "Please pay a total of $" << purchaseAmount + stateTax + localTax + levy << endl;
            }
        } 
}


