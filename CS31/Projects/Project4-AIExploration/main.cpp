//
//  main.cpp
//  Project 4 ChatGPT
//
//  Created by Duncan Hackmann on 3/1/23.
//

#include <iostream>
#include <string>

using namespace std;




int locateMaximum(const string array[], int n) {
    if (n <= 0) {
        return -1;
    }
    int maxIndex = 0;
    for (int i = 1; i < n; i++) {
        if (array[i] > array[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

int countFloatingPointValues(const string array[], int n) {
    if (n <= 0) {
        return -1;
    }
    int count = 0;
    for (int i = 0; i < n; i++) {
        bool isFloatingPoint = true;
        int dotCount = 0;
        for (char c : array[i]) {
            if (c == '.') {
                dotCount++;
            } else if (c < '0' || c > '9') {
                isFloatingPoint = false;
                break;
            }
        }
        if (isFloatingPoint && dotCount <= 1) {
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
        for (char c : array[i]) {
            if (c >= 'A' && c <= 'Z') {
                return false;
            }
        }
    }
    return true;
}

int shiftLeft(string array[], int n, int amount, string placeholder) {
    for (int i = 0; i < 5; i++) {
            cout << array[i] << ' ';
        }
    cout << endl;
    
    if (n <= 0 || amount < 0) {
        return -1;
    }
    int placeholderCount = 0;
    for (int i = 0; i < n; i++) {
        if (i + amount < n) {
            array[i] = array[i + amount];
        } else {
            array[i] = placeholder;
            placeholderCount++;
        }
    }
    
    for (int i = 0; i < 5; i++) {
            cout << array[i] << ' ';
        }
    cout << endl;
    
    return placeholderCount;
}

int main(int argc, const char * argv[]) {
    string folks[8] = {  "samwell", "jon", "margaery", "daenerys", "tyrion", "sansa", "magdalena", "jon" };
    string data[5] = { "mamaBbcca", "mamaBbcca", "12,", "98.76",
    "tyrion" };
    
    cout << shiftLeft( data, 5, -5,
                      "foobar" ) << endl;
}
