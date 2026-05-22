//
//  main.cpp
//  Project 3
//
//  Created by Duncan Hackmann on 2/4/23.
//
#include <string>
#include <iostream>
using namespace std;

bool isValidQC(string results);
int extractNumber(string results, int index);
int passQC(string results);
int defectQC(string results);
int totalQC(string results);
int batches(string results);

int main(int argc, const char * argv[]) {
    
    string results;
    
    while ( 1>0) {
        getline(cin, results);
        
        
        cout << isValidQC(results) << endl;
        cout << passQC(results) << endl;
        cout << defectQC(results) << endl;
        cout << totalQC(results) << endl;
        cout << batches(results) << endl;
    }
    return 0;
}


//tests whether or not the string results is valid
bool isValidQC(string results) {
    if (results[0] != 'Q') {
        return false;
    }
    
    int numQ = 0;
    int numD = 0;
    int numP = 0;
    
    
    //this loop traverses the string results and makes sure that there are no characters other than digits, Q, d, and p
    for (int i = 0; i < results.length(); i++) {
        
        if (!(results[i] == 'Q' || results[i] == 'd' || results[i] == 'p' || isdigit(results[i]))) {
            return false;
        }
        
        
        if (results[i] == 'Q') {
            numQ++;
        }
        if (results[i] == 'p') {
            numP++;
        }
        if (results[i] == 'd') {
            numD++;
        }
    }
    
    //makes sure that there are the correct number of p's and d's for the amount of Q's
    if (numP != numQ || numP != numD || numQ != numD) {
        return false;
    }
    
    //this outer for loop repeats for the number of times that there are batches
    for (int i = 0; i < numQ; i++) {
        int valQ = -1;
        int valD = -1;
        int valP = -1;
        
        //this for loop traverses the string results
        for (int j = 0; j < results.length(); j++) {
            if (results[j] == 'Q' && isdigit(results[j+1]) == true) {
        
                valQ = extractNumber(results, j+1);
                
                //valQ must have an updated value for the string to be valid
                if (valQ < 1) {
                    return false;
                }
            }
            
            if (results[j] == 'd' && isdigit(results[j+1]) == true) {
            
                valD = extractNumber(results, j+1);
                
                if (valD < 1) {
                    return false;
                }
            }
            
            if (results[j] == 'p' && isdigit(results[j+1]) == true) {
                valP = extractNumber(results, j+1);
                
                if (valP < 1) {
                    return false;
                }
            }
            //if, while traversing the string, there is a new batch, results is updated to not include what has already been processed, and the inner for loop will be broken out of
            if (results[j+1] == 'Q') {
                results = results.substr(j+1, results.length());
                break;
            }
        }
        if (valP + valD != valQ) {
            return false;
        }
    }
    return true;
}

//extracts a number from the string at a given index
int extractNumber(string results, int index) {
    int quantity = (0);
    
   
    
    if (index < results.length()) {
        
        if (results[index] >= '1' && results[index] <= '9') {
            
            while (index < results.length() && isdigit(results[index])) {
                int digit = results[index] - '0';
                quantity = quantity * 10 + digit;
                index = index + 1;
            }
        }
    }
    
    return quantity;
}

//this function counts the number of passes in the string results
int passQC(string results) {
    
    if (isValidQC(results) == false) {
        return -1;
    }
    
    int pVal = 0;
    
    for (int i = 0; i < results.length(); i++) {
        if (results[i] == 'p') {
            pVal += extractNumber(results, i+1);
        }
    }
    return pVal;
}

//this function counts the number of defects in the string results
int defectQC(string results) {
    
    if (isValidQC(results) == false) {
        return -1;
    }
    
    int dVal = 0;
    
    for (int i = 0; i < results.length(); i++) {
        if (results[i] == 'd') {
            dVal += extractNumber(results, i+1);
        }
    }
    return dVal;
}

//this function counts the number of tests in the string results
int totalQC(string results) {
    
    if (isValidQC(results) == false) {
        return -1;
    }
    return defectQC(results) + passQC(results);
}


//this function counts the number of batches in the string results
int batches(string results) {
    
    if (isValidQC(results) == false) {
        return -1;
    }
    int qVal = 0;
    for (int i = 0; i < results.length(); i++) {
        if (results[i] == 'Q') {
            qVal ++;
        }
    }
    return qVal;
}

