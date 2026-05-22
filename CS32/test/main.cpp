//
//  main.cpp
//  test
//
//  Created by Duncan Hackmann on 5/2/23.
//

#include <iostream>
using namespace std;

int countNums(int arr[], int size, int val) {
    if (size == 0) {
        return 0;
    }
    
    int total = countNums(arr +1, size-1, val);
    if (arr[0] == val) {
        total++;
    }
    return total;
}

int main()
{
    const int size = 5;
    int arr[size] = {7, 9, 6, 7, 7};
    cout << countNums(arr,size,7);
    // should print 3
}
