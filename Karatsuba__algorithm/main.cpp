#include <tuple>
#include <cmath>
#include <iostream>

#include "ArbitraryInt.h"

using namespace std;

int max (int a, int b) {
    return (a > b) ? a : b;
}


ArbitraryInt karatsuba_algorithm (const ArbitraryInt & a, const ArbitraryInt & b) {
    
    if (a.number_length() < 2 || b.number_length() < 2) {
        return ArbitraryInt(a.to_integer() * b.to_integer());
    }
    
    int size_max = (int) max(a.number_length(), b.number_length());
    int half = size_max / 2;
    
    ArbitraryInt high_a, low_a, high_b, low_b;
    tie(high_a, low_a) = split_integer_at(a, half);
    tie(high_b, low_b) = split_integer_at(b, half);
    
    ArbitraryInt z0, z1, z2;
    
    z0 = karatsuba_algorithm(low_a, low_b);
    z1 = karatsuba_algorithm(low_a + high_a, low_b + high_b);
    z2 = karatsuba_algorithm(high_a, high_b);
    
    return shift(z2, 2 * half) + shift(z1 - z2 - z0, half) + z0;
}


int main () {
    string a, b;
    cout << "Enter two numbers:" << endl;
    while (cin >> a >> b) {
        cout << karatsuba_algorithm(ArbitraryInt(a),
                                    ArbitraryInt(b)).repr() << endl;
    }
}
