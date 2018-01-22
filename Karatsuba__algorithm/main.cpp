#include <tuple>
#include <cmath>
#include <iostream>

#include "Karatsuba.h"

using namespace std;

int main () {
    string a, b;
    cout << "Enter two numbers:" << endl;
    while (cin >> a >> b) {
        cout << karatsuba_algorithm(ArbitraryInt(a),
                                    ArbitraryInt(b)).repr() << endl;
    }
}
