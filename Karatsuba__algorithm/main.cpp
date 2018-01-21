#include <tuple>
#include <cmath>
#include <iostream>

using namespace std;

size_t integer_length (long long number) {
    size_t digits = 0;
    
    while (number) {
        number /= 10;
        digits++;
    }
    
    return digits;
}

pair<long long, long long> split_integer_at (long long number, int position) {
    long long high, low, base_power;
    
    base_power = (long long) pow(10, position);
    
    high = number / base_power;
    low = number % base_power;
    
    return make_pair(high, low);
}

long long max (long long a, long long b) {
    return (a > b) ? a : b;
}


long long karatsuba_algorithm (long long a, long long b) {
    
    if (a < 10 || b < 10) {
        return a * b;
    }
    
    int size_max = (int) max(integer_length(a), integer_length(b));
    int half = size_max / 2;
    
    long long high_a, low_a, high_b, low_b;
    tie(high_a, low_a) = split_integer_at(a, half);
    tie(high_b, low_b) = split_integer_at(b, half);
    
    long long z0, z1, z2;
    
    z0 = karatsuba_algorithm(low_a, low_b);
    z1 = karatsuba_algorithm(low_a + high_a, low_b + high_b);
    z2 = karatsuba_algorithm(high_a, high_b);
    
    return (z2 * pow(10, 2 * half)) + ((z1 - z2 - z0) * pow(10, half)) + z0;
}


int main () {
    long long a, b;
    cout << "Enter two numbers:" << endl;
    while (cin >> a >> b) {
        cout << karatsuba_algorithm(a, b) << endl;
    }
}

