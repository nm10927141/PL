#include <iostream>
using namespace std;

int main() {
    int b = 0;
    int a = 3;
    cout << (a = a) << endl;

    cout << (b = b ++ ) << endl;

    return 0;

}