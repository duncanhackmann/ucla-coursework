// Electoral College
#include <iostream>
#include <cassert>
using namespace std;

bool foundAllThree;
bool answer;
int i;
int j;
int k;
int allAsBeforeBsBeforeCs( string s, bool & foundAllThree);
void hasThatManyOfThis( string s, char thatCharacter, char thisCharacter, bool & answer );
void foo( int & i, int & j, int k );

int main()
{
    int a = 5;
    int b = 10;
    
    do
     {
        cin >> b;
        cin >> a;
        cout << "i = " << a << " j = " << b << endl;
     } while ( b > a );
    
    cout << endl << "-------" << endl;
    
    int i = 5;
    int j = 10;
    
    for (int c = i; i < j; c++) {
        cin >> j;
        cin >> i;
        cout << "i = " << i << " j = " << j << endl;
    }
}
    







void foo( int & i, int & j, int k ) {
    i = -1;
    &k = -1;
    cout << i << endl;
    cout << k << endl;
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
//    int a = 10;
//    int b = 5;
//
//    while( a < b )
//      {
//         cin >> b;
//         cin >> a;
//         cout << "a = " << a << " b = " << b << endl;
//      }
//
//    cout << endl << "DO" << endl;
//
//    int i = 10;
//    int j = 5;
//
//    do {
//        if (i >= j) {
//            break;
//        }
//        cin >> j;
//        cin >> i;
//        cout << "i = " << i << " j = " << j << endl;
//
//    } while (i < j);
//
    

//void hasThatManyOfThis( string s, char thatCharacter, char thisCharacter, bool & answer ) {
//    if (s == "") {
//        answer = false;
//    }
//
//    int numThis = 0;
//    int numThat = 0;
//
//    for (int i = 0; i < s.length(); i++) {
//        if (s[i] == thatCharacter) {
//            numThat++;
//        }
//        if (s[i] == thisCharacter) {
//            numThis++;
//        }
//    }
//
//    if (numThis == numThat) {
//        answer = true;
//    } else {
//        answer = false;
//    }
//    return numThat+numThis;
//}


int allAsBeforeBsBeforeCs( string s, bool & foundAllThree) {
    if (s == "") {
        return -1;
    }
    
    int numABC = 0;
    
    foundAllThree = true;
    
    for (int i = 0; i < s.length(); i++) {

        if (s[i] == 'A') {
            numABC++;
            
        }
        
        if (s[i] == 'B') {
            numABC++;
            for (int j = i; j < s.length(); j++) {
                if (s[j] == 'A') {
                    foundAllThree = false;
                }
            }
        }
        
        if (s[i] == 'C') {
            numABC++;
            for (int j = i; j < s.length(); j++) {
                if (s[j] == 'A' || s[j] == 'B') {
                   foundAllThree = false;
                }
            }
        }
    }
    
    if (numABC == 0) {
        foundAllThree = false;
    }
    
    return numABC;
}
