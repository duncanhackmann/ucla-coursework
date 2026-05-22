//
//  linear.cpp
//  Homework 3
//
//  Created by Duncan Hackmann on 5/10/23.
//

bool allTrue(const string a[], int n) {
    if (n <= 0) {
        return true;
    }
    return (somePredicate(a[n - 1]) && allTrue(a, n - 1));
}

int countTrue(const string a[], int n) {
    if (n <= 0) {
        return 0;
    }
    int count = countTrue(a, n - 1);
    if (somePredicate(a[n - 1])) {
        count++;
    }
    return count;
}

int firstTrue(const string a[], int n) {
    if (n <= 0) {
        return -1;
    }
    int index = firstTrue(a, n - 1);
    if (index == -1 && somePredicate(a[n - 1])) {
        index = n - 1;
    }
    return index;
}

int positionOfMax(const string a[], int n) {
    if (n <= 0) {
        return -1;
    }
    int index = positionOfMax(a, n - 1);
    if (index == -1 || a[n - 1] >= a[index]) {
        index = n - 1;
    }
    return index;
}

bool contains(const string a1[], int n1, const string a2[], int n2) {
    if (n2 <= 0) {
        return true;
    }
    if (n1 <= 0) {
        return false;
    }
    if (a1[0] == a2[0]) {
        return contains(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
    }
    return contains(a1 + 1, n1 - 1, a2, n2);
}


