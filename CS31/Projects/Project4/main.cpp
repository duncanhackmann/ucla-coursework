//
//  main.cpp
//  Project 4
//
//  Created by Duncan Hackmann on 2/22/23.
//
#include <string>
#include<array>
#include <iostream>
using namespace std;

int locateMaximum(const string array[], int n);
int countFloatingPointValues(const string array[], int n);
bool hasNoCapitals(const string array[], int n);
int shiftLeft(string array[], int n, int amount, string placeholder);

int main(int argc, const char * argv[]) {
    
    string data[5] = { "mamaBbcca", "mamaBbcca", "12,", "98.76",
    "." };
    cout << countFloatingPointValues( data, 5 );
}


int locateMaximum(const string array[], int n) {
    if (n <= 0) {
        return -1;
    }
    
    int index = 0;
    string max = "";
    
    //traverses the array and checks each element to see if it is bigger than max and updates max accordingly
    for (int i = 0; i < n; i++) {
        if (array[i] > max) {
            max = array[i];
            index = i;
        }
    }
    return index;
}

int countFloatingPointValues(const string array[], int n) {
    if (n <= 0) {
        return -1;
    }
    
    int count = 0;
    
    //traverses the array
    for (int i = 0; i < n; i++) {
        //variable x tracks whether or not the current element is valid
        int x = 1;
        
        //without this, code does not work because isdigit reads an empty string as a digit
        if (array[i] == "") {
            x *= 0;
        }
        
        if (array[i] == ".") {
            x *= 0;
        }
        
        int decimalCount = 0;
        
        //traverses each character of the current element
        for (int j = 0; j < array[i].length(); j++) {
             
            \
            //checks that current char is a digit or decimal point
            if (!(isdigit(array[i][j]) || array[i][j] == '.')) {
                x *= 0;
            }
            
            
            if (array[i][j] == '.') {
                decimalCount++;
            }
        }
        //makes sure there is only one decimal point
        if (decimalCount > 1) {
            x *= 0;
        }
        
        if (x == 1) {
            count++;
        }
    
    }
    return count;
}


bool hasNoCapitals(const string array[], int n) {
    if (n <= 0) {
        return true;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < array[i].length(); j++) {
            if (array[i][j] >= 'A' && array[i][j] <= 'Z') {
                return false;
            }
        }
    }
    return true;
}

int shiftLeft(string array[], int n, int amount, string placeholder) {
    
    if (n <= 0 || amount < 0) {
        return -1;
    }
    
    int count = 0;
    
    //if n <= amount, the full array will be filled with the placeholder, so this skips the for loop and allows the whole array to be filled with the placeholder
    if (n <= amount) {
        amount = -1;
    }
    
    //adjusts the elements of the array to the left by the correct amount
    for (int i = 0; i <= amount; i++) {
        array[i] = array[i+amount];
    }
        
    //fills the rest of the array with placeholders
    for (int j = amount+1; j < n; j++) {
        array[j] = placeholder;
        count++;
    }
    
    return count;
}

