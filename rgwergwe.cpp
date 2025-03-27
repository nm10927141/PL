#include <stdio.h>
#include <iostream>
using namespace std;
int F1(int a) {
    return a*2;
}
int F2( int a ){
    return a*3;
}

int main() {
    int x = 5;
    int& ref = x;
    cout << ref << &ref  << endl;
    return 0;
}